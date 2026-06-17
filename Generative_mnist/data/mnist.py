"""MNIST 数据集下载、加载与可视化若干样本。

提供 get_dataloaders() 返回 train/val/test 的 DataLoader。
默认将数据与缓存放在 experiments/data/ 目录。
"""
from __future__ import annotations
import os
from typing import Tuple

import torch
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms, utils as vutils

from pytorch_generative_mnist.config import Config, ensure_dirs


def get_transforms():
    """返回训练/测试统一的 transforms。

    - GAN 使用 [-1,1] 范围便于 Tanh；AE 在 MSE 下也可用该范围，若用 BCE，模型内部用 logits 预测。
    """
    return transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,)),  # 将 [0,1] 映射到 [-1,1]
    ])


def get_datasets(cfg: Config):
    ensure_dirs(cfg)
    transform = get_transforms()
    root = cfg.data_dir
    train_full = datasets.MNIST(root=root, train=True, transform=transform, download=True)
    test = datasets.MNIST(root=root, train=False, transform=transform, download=True)

    # 从训练集划一部分做验证集（例如 5k）
    val_size = 5000
    train_size = len(train_full) - val_size
    train, val = random_split(train_full, [train_size, val_size], generator=torch.Generator().manual_seed(cfg.seed))
    return train, val, test


def get_dataloaders(cfg: Config) -> Tuple[DataLoader, DataLoader, DataLoader]:
    """构造 DataLoader。
    Args:
        cfg: 全局配置
    Returns:
        (train_loader, val_loader, test_loader)
    """
    train, val, test = get_datasets(cfg)
    train_loader = DataLoader(train, batch_size=cfg.batch_size, shuffle=True, num_workers=cfg.num_workers, pin_memory=True)
    val_loader = DataLoader(val, batch_size=cfg.batch_size, shuffle=False, num_workers=cfg.num_workers, pin_memory=True)
    test_loader = DataLoader(test, batch_size=cfg.batch_size, shuffle=False, num_workers=cfg.num_workers, pin_memory=True)
    return train_loader, val_loader, test_loader


def visualize_batch(cfg: Config, num_images: int = 64, split: str = "train") -> str:
    """保存若干样本网格，返回保存路径。"""
    from torch.utils.data import Subset

    train, val, test = get_datasets(cfg)
    ds = train if split == "train" else (val if split == "val" else test)
    idxs = list(range(min(num_images, len(ds))))
    subset = Subset(ds, idxs)
    loader = DataLoader(subset, batch_size=num_images, shuffle=False)
    imgs, labels = next(iter(loader))
    grid = vutils.make_grid(imgs, nrow=int(num_images ** 0.5), normalize=True, value_range=(-1, 1))
    save_path = os.path.join(cfg.samples_dir, f"mnist_{split}_samples.png")
    vutils.save_image(grid, save_path)
    return save_path

