"""DCGAN 风格生成器与判别器（MNIST 1x28x28）。

- 生成器：z -> 1x28x28（通过转置卷积上采样，末层 tanh）
- 判别器：1x28x28 -> 标量 logits（BCEWithLogitsLoss）
"""
from __future__ import annotations
import torch
import torch.nn as nn


class Generator(nn.Module):
    def __init__(self, z_dim: int = 100, base_channels: int = 64):
        super().__init__()
        self.fc = nn.Linear(z_dim, base_channels * 4 * 4)
        self.net = nn.Sequential(
            nn.ConvTranspose2d(base_channels, base_channels // 2, 4, 2, 1),  # 8x8
            nn.BatchNorm2d(base_channels // 2),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels // 2, base_channels // 4, 4, 2, 1),  # 16x16
            nn.BatchNorm2d(base_channels // 4),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels // 4, base_channels // 8, 4, 2, 1),  # 32x32
            nn.BatchNorm2d(base_channels // 8),
            nn.ReLU(True),
            nn.Conv2d(base_channels // 8, 1, kernel_size=5, padding=2),
            nn.Tanh(),  # 输出范围 [-1,1]
        )

    def forward(self, z):
        h = self.fc(z)
        h = h.view(h.size(0), -1, 4, 4)
        x = self.net(h)
        x = x[:, :, :28, :28]  # 裁到 28x28
        return x


class Discriminator(nn.Module):
    def __init__(self, base_channels: int = 64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv2d(1, base_channels // 2, 4, 2, 1),  # 14x14
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels // 2, base_channels, 4, 2, 1),  # 7x7
            nn.BatchNorm2d(base_channels),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels, base_channels * 2, 3, 2, 1),  # 4x4
            nn.BatchNorm2d(base_channels * 2),
            nn.LeakyReLU(0.2, inplace=True),
        )
        self.head = nn.Linear(base_channels * 2 * 4 * 4, 1)

    def forward(self, x):
        h = self.net(x)
        h = h.view(h.size(0), -1)
        logits = self.head(h)  # 未经过 sigmoid 的 logits
        return logits

