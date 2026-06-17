"""评估指标接口与占位实现。

- Inception Score (IS) / Fréchet Inception Distance (FID) 的真实实现依赖较多且开销较大。
- 这里提供占位函数与接口，返回可用的标量以便示例训练流程不报错。
- 如需真实指标，可参考 pytorch-fid 或 torchmetrics 替换。
"""
from __future__ import annotations
import torch
from typing import Tuple


def inception_score_placeholder(fake_imgs: torch.Tensor) -> Tuple[float, float]:
    """返回占位 IS 均值/方差。实际中应使用预训练 Inception 网络。"""
    # 简单以像素方差为伪指标（教学演示用，不具备实际意义）
    v = float(fake_imgs.var().item())
    return 5.0 + v, 0.1


def fid_placeholder(fake_imgs: torch.Tensor, real_imgs: torch.Tensor) -> float:
    """返回占位 FID。实际中应使用 Inception 统计均值/协方差计算。"""
    return float((fake_imgs.mean() - real_imgs.mean()).abs().item())

