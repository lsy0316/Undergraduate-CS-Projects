"""CelebA 数据集支持（torchvision.datasets.CelebA）。

功能：
- 自动下载（首次运行）并加载对齐裁剪版 CelebA
- 构造 train/val/test 的 DataLoader
- 提供可视化若干样本的函数
- 可选将某个属性（如 "Smiling"）作为二分类标签，便于演示 cGAN；若不需要，返回占位 0 标签

注意：
- CelebA 是三通道彩色图像，建议使用 img_size=64（典型 DCGAN 设置）
- 输出的像素范围经 Normalize 到 [-1, 1]
"""
from __future__ import annotations
import os
from typing import Tuple, Optional

import torch
from torch.utils.data import DataLoader
from torchvision import datasets, transforms, utils as vutils

from pytorch_generative_mnist.config import Config, ensure_dirs


_CELEBA_ATTRS = [
    # 官方 40 个属性名称（按官方顺序）
    "5_o_Clock_Shadow", "Arched_Eyebrows", "Attractive", "Bags_Under_Eyes", "Bald",
    "Bangs", "Big_Lips", "Big_Nose", "Black_Hair", "Blond_Hair", "Blurry", "Brown_Hair",
    "Bushy_Eyebrows", "Chubby", "Double_Chin", "Eyeglasses", "Goatee", "Gray_Hair",
    "Heavy_Makeup", "High_Cheekbones", "Male", "Mouth_Slightly_Open", "Mustache",
    "Narrow_Eyes", "No_Beard", "Oval_Face", "Pale_Skin", "Pointy_Nose", "Receding_Hairline",
    "Rosy_Cheeks", "Sideburns", "Smiling", "Straight_Hair", "Wavy_Hair", "Wearing_Earrings",
    "Wearing_Hat", "Wearing_Lipstick", "Wearing_Necklace", "Wearing_Necktie", "Young",
]


def get_transforms(img_size: int = 64):
    """CelebA 常用预处理：Resize->CenterCrop->ToTensor->Normalize([-1,1])."""
    return transforms.Compose([
        transforms.Resize(img_size),
        transforms.CenterCrop(img_size),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
    ])


def _resolve_attr_index(attr_name: Optional[str]) -> Optional[int]:
    if attr_name is None:
        return None
    try:
        return _CELEBA_ATTRS.index(attr_name)
    except ValueError:
        raise ValueError(f"未知属性名: {attr_name}. 可选: {', '.join(_CELEBA_ATTRS[:10])} ... 共40个")


def get_datasets(cfg: Config, img_size: int = 64, target_attr: Optional[str] = None):
    """加载 CelebA 的 train/valid/test split。
    target_attr: 若提供，将作为标签（0/1），否则返回 0 占位标签。
    """
    ensure_dirs(cfg)
    transform = get_transforms(img_size)
    root = cfg.data_dir
    # torchvision >=0.15: split in {"train","valid","test"}
    train = datasets.CelebA(root=root, split="train", target_type="attr", transform=transform, download=True)
    val = datasets.CelebA(root=root, split="valid", target_type="attr", transform=transform, download=True)
    test = datasets.CelebA(root=root, split="test", target_type="attr", transform=transform, download=True)

    # 包装：将 40 维属性映射为所需标签
    attr_idx = _resolve_attr_index(target_attr)

    def _map_label(attr_tensor: torch.Tensor) -> torch.Tensor:
        if attr_idx is None:
            return torch.tensor(0, dtype=torch.long)  # 占位 0 标签
        # 官方属性为 {-1, 1}，我们映射到 {0,1}
        v = attr_tensor[attr_idx].item()
        return torch.tensor(1 if v > 0 else 0, dtype=torch.long)

    # 使用 Subset-like 包装以转换标签
    class _Wrap(torch.utils.data.Dataset):
        def __init__(self, base):
            self.base = base
        def __len__(self):
            return len(self.base)
        def __getitem__(self, idx):
            img, attrs = self.base[idx]
            return img, _map_label(attrs)

    return _Wrap(train), _Wrap(val), _Wrap(test)


def get_dataloaders(cfg: Config, img_size: int = 64, target_attr: Optional[str] = None) -> Tuple[DataLoader, DataLoader, DataLoader]:
    """构造 CelebA DataLoader（默认 img_size=64）。"""
    train, val, test = get_datasets(cfg, img_size=img_size, target_attr=target_attr)
    train_loader = DataLoader(train, batch_size=cfg.batch_size, shuffle=True, num_workers=cfg.num_workers, pin_memory=True)
    val_loader = DataLoader(val, batch_size=cfg.batch_size, shuffle=False, num_workers=cfg.num_workers, pin_memory=True)
    test_loader = DataLoader(test, batch_size=cfg.batch_size, shuffle=False, num_workers=cfg.num_workers, pin_memory=True)
    return train_loader, val_loader, test_loader


def visualize_batch(cfg: Config, num_images: int = 64, split: str = "train", img_size: int = 64, target_attr: Optional[str] = None) -> str:
    """保存 CelebA 若干样本网格图，返回保存路径。"""
    from torch.utils.data import Subset

    train, val, test = get_datasets(cfg, img_size=img_size, target_attr=target_attr)
    ds = train if split == "train" else (val if split == "val" else test)
    idxs = list(range(min(num_images, len(ds))))
    subset = Subset(ds, idxs)
    loader = DataLoader(subset, batch_size=num_images, shuffle=False)
    imgs, labels = next(iter(loader))
    grid = vutils.make_grid(imgs, nrow=int(num_images ** 0.5), normalize=True, value_range=(-1, 1))
    subdir = os.path.join(cfg.samples_dir)
    os.makedirs(subdir, exist_ok=True)
    save_path = os.path.join(subdir, f"celeba_{split}_samples.png")
    vutils.save_image(grid, save_path)
    return save_path

