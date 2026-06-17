"""可视化工具：保存网格图、绘制 loss 曲线、TensorBoard 可选。
"""
from __future__ import annotations
import os
from typing import Dict, List, Optional

import matplotlib.pyplot as plt
import torch
from torchvision.utils import make_grid, save_image


class TBLogger:
    """一个简单的 TensorBoard 包装器，若导入失败则降级为 no-op。"""
    def __init__(self, log_dir: str, enable: bool = True):
        self.writer = None
        if enable:
            try:
                from torch.utils.tensorboard import SummaryWriter
                self.writer = SummaryWriter(log_dir=log_dir)
            except Exception:
                self.writer = None

    def add_scalar(self, tag: str, value: float, step: int):
        if self.writer:
            self.writer.add_scalar(tag, value, step)

    def close(self):
        if self.writer:
            self.writer.close()


def save_grid(images, path: str, nrow: int = 8, normalize: bool = True, value_range=(-1, 1)):
    grid = make_grid(images, nrow=nrow, normalize=normalize, value_range=value_range)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    save_image(grid, path)


def plot_losses(loss_dict: Dict[str, List[float]], save_path: str):
    """将若干 loss 曲线绘制到一张图中并保存。"""
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    plt.figure(figsize=(8, 5))
    for k, v in loss_dict.items():
        if len(v) == 0:
            continue
        plt.plot(v, label=k)
    plt.xlabel("Step")
    plt.ylabel("Loss")
    plt.legend()
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()


def save_recon_comparison(orig: torch.Tensor, recon: torch.Tensor, save_path: str, title: str = ""):
    """将重建结果与原图以更直观的方式排列并保存：
    - 每个样本的原图与重建图左右并排，并在中间加入“分隔线”，更易对比
    - 自动生成一个网格（每行若干对），并在图标题中写入指标
    Args:
        orig: 原始图像 (N,1,28,28)，范围[-1,1]或[0,1]
        recon: 重建图像 (N,1,28,28)，范围[-1,1]或[0,1]
        save_path: 保存路径
        title: 额外标题（可包含 epoch / 平均重建损失等）
    """
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    n = min(orig.size(0), recon.size(0))

    paired_images = []
    sep_w = 2  # 分隔线宽度（像素）
    for i in range(n):
        oi = orig[i]
        ri = recon[i]
        # 生成竖直分隔线（值为 1.0，对应可视化白色；图像范围为[-1,1]时会被 normalize 处理）
        h = oi.size(1)
        sep = torch.ones((oi.size(0), h, sep_w), dtype=oi.dtype, device=oi.device)
        pair = torch.cat([oi, sep, ri], dim=2)  # 在宽度维拼接：原图 | 分隔线 | 重建
        paired_images.append(pair)

    # 每行显示的“对”数量（非单张），例如 10 对/行
    nrow = 10
    grid = make_grid(paired_images, nrow=nrow, normalize=True, value_range=(-1, 1), padding=2)

    # 使用 matplotlib 保存并带标题
    plt.figure(figsize=(12, 6))
    plt.axis('off')
    if title:
        plt.title(title)
    plt.imshow(grid.permute(1, 2, 0).cpu().numpy())
    plt.tight_layout()
    plt.savefig(save_path, bbox_inches='tight', pad_inches=0.1)
    plt.close()


def plot_train_val_curves(train_losses: List[float], val_losses: List[float], save_path: str, xlabel: str = "Epoch", ylabel: str = "Loss", title: str = "Training/Validation Loss"):
    """绘制训练/验证曲线（按 epoch 统计的平均损失）。"""
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    plt.figure(figsize=(7, 5))
    if train_losses:
        plt.plot(train_losses, label="train")
    if val_losses:
        plt.plot(val_losses, label="val")
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()


