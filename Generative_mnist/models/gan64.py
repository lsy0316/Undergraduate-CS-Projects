"""DCGAN for CelebA 64x64x3.

- Generator64: z -> 3x64x64 (Tanh)
- Discriminator64: 3x64x64 -> logits
"""
from __future__ import annotations
import torch
import torch.nn as nn


class Generator64(nn.Module):
    def __init__(self, z_dim: int = 100, base_channels: int = 64, out_channels: int = 3):
        super().__init__()
        self.proj = nn.Linear(z_dim, base_channels * 8 * 4 * 4)
        self.net = nn.Sequential(
            # input: (bc*8) x 4 x 4
            nn.ConvTranspose2d(base_channels * 8, base_channels * 4, 4, 2, 1),  # 8x8
            nn.BatchNorm2d(base_channels * 4),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels * 4, base_channels * 2, 4, 2, 1),  # 16x16
            nn.BatchNorm2d(base_channels * 2),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels * 2, base_channels, 4, 2, 1),  # 32x32
            nn.BatchNorm2d(base_channels),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels, out_channels, 4, 2, 1),  # 64x64
            nn.Tanh(),
        )

    def forward(self, z: torch.Tensor) -> torch.Tensor:
        h = self.proj(z)
        h = h.view(h.size(0), -1, 4, 4)
        x = self.net(h)
        return x


class Discriminator64(nn.Module):
    def __init__(self, in_channels: int = 3, base_channels: int = 64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv2d(in_channels, base_channels, 4, 2, 1),  # 32x32
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels, base_channels * 2, 4, 2, 1),  # 16x16
            nn.BatchNorm2d(base_channels * 2),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels * 2, base_channels * 4, 4, 2, 1),  # 8x8
            nn.BatchNorm2d(base_channels * 4),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels * 4, base_channels * 8, 4, 2, 1),  # 4x4
            nn.BatchNorm2d(base_channels * 8),
            nn.LeakyReLU(0.2, inplace=True),
        )
        self.head = nn.Linear(base_channels * 8 * 4 * 4, 1)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        h = self.net(x)
        h = h.view(h.size(0), -1)
        logits = self.head(h)
        return logits

