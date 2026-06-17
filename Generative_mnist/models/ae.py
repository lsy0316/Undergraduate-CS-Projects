"""AutoEncoder (AE) 模型：轻量 CNN 编码器/解码器。

- 输入: (N,1,28,28)
- 潜变量: latent_dim (默认 64)
- 重构损失: BCEWithLogitsLoss 或 MSELoss（通过 config 选择）
"""
from __future__ import annotations
import torch
import torch.nn as nn


class Encoder(nn.Module):
    """卷积编码器，将 1x28x28 -> latent_dim。

    使用步幅卷积实现下采样，最后自适应到向量。
    """
    def __init__(self, latent_dim: int = 64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=4, stride=2, padding=1),  # 14x14
            nn.ReLU(inplace=True),
            nn.Conv2d(32, 64, kernel_size=4, stride=2, padding=1),  # 7x7
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.Conv2d(64, 128, kernel_size=3, stride=2, padding=1),  # 4x4
            nn.BatchNorm2d(128),
            nn.ReLU(inplace=True),
        )
        self.proj = nn.Linear(128 * 4 * 4, latent_dim)

    def forward(self, x):
        h = self.net(x)
        h = h.view(h.size(0), -1)
        z = self.proj(h)  # 潜变量表示（无激活）
        return z


class Decoder(nn.Module):
    """卷积解码器，将 latent_dim -> 1x28x28 logits。

    末层不加激活（logits），以便：
    - BCEWithLogitsLoss 直接使用；
    - 若用 MSELoss，训练时可以对输出 tanh 以映射到 [-1,1]（见训练循环实现）。
    """
    def __init__(self, latent_dim: int = 64):
        super().__init__()
        self.fc = nn.Linear(latent_dim, 128 * 4 * 4)
        self.net = nn.Sequential(
            nn.ConvTranspose2d(128, 64, kernel_size=4, stride=2, padding=1),  # 8x8
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.ConvTranspose2d(64, 32, kernel_size=4, stride=2, padding=1),  # 16x16
            nn.BatchNorm2d(32),
            nn.ReLU(inplace=True),
            nn.ConvTranspose2d(32, 16, kernel_size=4, stride=2, padding=1),  # 32x32
            nn.BatchNorm2d(16),
            nn.ReLU(inplace=True),
            nn.Conv2d(16, 1, kernel_size=5, padding=2),  # 28x28 (通过卷积回到 28)
        )

    def forward(self, z):
        h = self.fc(z)
        h = h.view(h.size(0), 128, 4, 4)
        x_logits = self.net(h)
        x_logits = x_logits[:, :, :28, :28]  # 裁到 28x28
        return x_logits


class AutoEncoder(nn.Module):
    def __init__(self, latent_dim: int = 64):
        super().__init__()
        self.enc = Encoder(latent_dim)
        self.dec = Decoder(latent_dim)

    def forward(self, x):
        z = self.enc(x)
        x_logits = self.dec(z)
        return x_logits, z

