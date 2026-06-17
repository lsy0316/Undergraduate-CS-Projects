"""Conditional DCGAN for CelebA 64x64x3.

- ConditionalGenerator64: concat z with label embedding
- ConditionalDiscriminator64: concat condition map with image (channel-wise)

Note: CelebA attributes are multi-label,这里简化为选择一个二分类属性（如 Smiling）作为条件 y∈{0,1}。
"""
from __future__ import annotations
import torch
import torch.nn as nn


class ConditionalGenerator64(nn.Module):
    def __init__(self, z_dim: int = 100, num_classes: int = 2, embed_dim: int = 20, base_channels: int = 64, out_channels: int = 3):
        super().__init__()
        self.embed = nn.Embedding(num_classes, embed_dim)
        self.proj = nn.Linear(z_dim + embed_dim, base_channels * 8 * 4 * 4)
        self.net = nn.Sequential(
            nn.ConvTranspose2d(base_channels * 8, base_channels * 4, 4, 2, 1),
            nn.BatchNorm2d(base_channels * 4),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels * 4, base_channels * 2, 4, 2, 1),
            nn.BatchNorm2d(base_channels * 2),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels * 2, base_channels, 4, 2, 1),
            nn.BatchNorm2d(base_channels),
            nn.ReLU(True),
            nn.ConvTranspose2d(base_channels, out_channels, 4, 2, 1),
            nn.Tanh(),
        )

    def forward(self, z: torch.Tensor, y: torch.Tensor) -> torch.Tensor:
        y_emb = self.embed(y)
        h = torch.cat([z, y_emb], dim=1)
        h = self.proj(h)
        h = h.view(h.size(0), -1, 4, 4)
        x = self.net(h)
        return x


class ConditionalDiscriminator64(nn.Module):
    def __init__(self, num_classes: int = 2, embed_dim: int = 20, base_channels: int = 64, in_channels: int = 3):
        super().__init__()
        self.embed = nn.Embedding(num_classes, embed_dim)
        self.cond_proj = nn.Linear(embed_dim, 64 * 64)  # to 1x64x64 map
        self.net = nn.Sequential(
            nn.Conv2d(in_channels + 1, base_channels, 4, 2, 1),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels, base_channels * 2, 4, 2, 1),
            nn.BatchNorm2d(base_channels * 2),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels * 2, base_channels * 4, 4, 2, 1),
            nn.BatchNorm2d(base_channels * 4),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(base_channels * 4, base_channels * 8, 4, 2, 1),
            nn.BatchNorm2d(base_channels * 8),
            nn.LeakyReLU(0.2, inplace=True),
        )
        self.head = nn.Linear(base_channels * 8 * 4 * 4, 1)

    def forward(self, x: torch.Tensor, y: torch.Tensor) -> torch.Tensor:
        y_emb = self.embed(y)
        cond = self.cond_proj(y_emb).view(x.size(0), 1, 64, 64)
        x_cond = torch.cat([x, cond], dim=1)
        h = self.net(x_cond)
        h = h.view(h.size(0), -1)
        logits = self.head(h)
        return logits

