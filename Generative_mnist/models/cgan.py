"""条件 GAN（cGAN）

类别变量 y 的使用说明（以 MNIST 为例，y ∈ {0..9}）：
- y 是整型类别索引（dtype=torch.long），形状为 [N]，每个取值在 [0, num_classes-1]。
- 生成器（G）：先将 y 通过 nn.Embedding 映射到连续向量 y_emb，再与噪声 z 在特征维拼接，得到 [N, z_dim+embed_dim] 作为输入；这样 G 能“按类别生成”。
- 判别器（D）：同样对 y 做嵌入，并线性投影到 28×28 的单通道条件图 cond_map；将 cond_map 与图像 x 在通道维拼接得到 [N, 2, 28, 28] 输入，D 由此判断“真实/生成”以及“是否与 y 匹配”。

训练时：
- 真样本对 (x_real, y_real) 来自数据集的图像与标签；
- 假样本对 (x_fake, y_fake) 则由 z∼N(0,1) 与采样的 y_fake 得到，其中 x_fake = G(z, y_fake)。

注意：
- y 的 dtype 必须是 torch.long（Embedding 的索引类型），且设备需与 z/x 一致；
- 若切换到其他数据集/尺寸，应相应调整 cond_map 的空间尺寸与通道数。
"""
from __future__ import annotations
import torch
import torch.nn as nn


class ConditionalGenerator(nn.Module):
    def __init__(self, z_dim: int = 100, num_classes: int = 10, embed_dim: int = 20, base_channels: int = 64):
        super().__init__()
        self.embed = nn.Embedding(num_classes, embed_dim)
        in_dim = z_dim + embed_dim  # 拼接 z 与 label 嵌入
        self.fc = nn.Linear(in_dim, base_channels * 4 * 4)
        self.net = nn.Sequential(
            nn.ConvTranspose2d(base_channels, base_channels // 2, 4, 2, 1),
            nn.BatchNorm2d(base_channels // 2),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels // 2, base_channels // 4, 4, 2, 1),
            nn.BatchNorm2d(base_channels // 4),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels // 4, base_channels // 8, 4, 2, 1),
            nn.BatchNorm2d(base_channels // 8),
            nn.ReLU(True),
            nn.Conv2d(base_channels // 8, 1, kernel_size=5, padding=2),
            nn.Tanh(),
        )

    def forward(self, z, y):
        """
        条件生成前向计算。

        Args:
            z: Tensor，形状 [N, z_dim]，标准高斯噪声。
            y: LongTensor，形状 [N]，每个取值在 [0, num_classes-1]，作为类别条件。
        Returns:
            x: Tensor，形状 [N, 1, 28, 28]，像素范围约为 [-1, 1]（Tanh 输出）。
        """
        # 1) 将离散类别 y 映射为连续向量（词向量式的 label 表示）
        y_emb = self.embed(y)                    # [N, embed_dim]
        # 2) 条件注入：与噪声 z 拼接，得到条件化输入
        h = torch.cat([z, y_emb], dim=1)         # [N, z_dim+embed_dim]
        # 3) 投影到卷积特征图，并通过反卷积链路上采样成图像
        h = self.fc(h)
        h = h.view(h.size(0), -1, 4, 4)
        x = self.net(h)
        # 4) 由于上采样为 32x32，这里裁剪到 28x28 以匹配 MNIST 尺寸
        x = x[:, :, :28, :28]
        return x


class ConditionalDiscriminator(nn.Module):
    def __init__(self, num_classes: int = 10, embed_dim: int = 20, base_channels: int = 64):
        super().__init__()
        self.embed = nn.Embedding(num_classes, embed_dim)
        # 将嵌入通过线性层映射到 28x28 的条件图
        self.cond_proj = nn.Linear(embed_dim, 28 * 28)
        self.net = nn.Sequential(
            nn.Conv2d(2, base_channels // 2, 4, 2, 1),  # 输入通道=图像1+条件1
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels // 2, base_channels, 4, 2, 1),
            nn.BatchNorm2d(base_channels),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels, base_channels * 2, 3, 2, 1),
            nn.BatchNorm2d(base_channels * 2),
            nn.LeakyReLU(0.2, inplace=True),
        )
        self.head = nn.Linear(base_channels * 2 * 4 * 4, 1)

    def forward(self, x, y):
        """
        条件判别前向计算。

        Args:
            x: Tensor，形状 [N, 1, 28, 28]，真实或生成图像（值域通常为 [-1, 1]）。
            y: LongTensor，形状 [N]，类别索引，与 x 的语义应当匹配。
        Returns:
            logits: Tensor，形状 [N, 1]，未经过 sigmoid 的判别分数。
        """
        # 1) y -> 嵌入向量 -> 投影为 28x28 的“条件图”（单通道）
        y_emb = self.embed(y)                                    # [N, embed_dim]
        cond = self.cond_proj(y_emb).view(x.size(0), 1, 28, 28)  # [N,1,28,28]
        # 2) 将条件图与输入图像在通道维拼接，形成 2 通道输入
        x_cond = torch.cat([x, cond], dim=1)                     # [N,2,28,28]
        # 3) 卷积判别，输出标量 logits
        h = self.net(x_cond)
        h = h.view(h.size(0), -1)
        logits = self.head(h)
        return logits

