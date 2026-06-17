"""Checkpoint 保存/恢复工具。

- 仅保存 last / best：不再保存每个 epoch 的快照文件
- 支持 resume：恢复模型、优化器、调度器与 epoch/step
"""
from __future__ import annotations
import os
from typing import Any, Dict
import torch


def save_checkpoint(state: Dict[str, Any], ckpt_dir: str, prefix: str, is_best: bool = False, epoch: int | None = None) -> str:
    """保存 checkpoint，仅保留 {prefix}_last.pth 与（可选）{prefix}_best.pth。

    注意：epoch 参数将被忽略，仅为兼容旧接口。
    """
    os.makedirs(ckpt_dir, exist_ok=True)
    last_path = os.path.join(ckpt_dir, f"{prefix}_last.pth")
    torch.save(state, last_path)
    if is_best:
        best_path = os.path.join(ckpt_dir, f"{prefix}_best.pth")
        torch.save(state, best_path)
    return last_path


def load_checkpoint(path: str, model: torch.nn.Module, optimizer: torch.optim.Optimizer | None = None, scheduler: Any | None = None, map_location: str | None = None) -> Dict[str, Any]:
    """加载 checkpoint 并恢复状态。返回 state 字典供外部使用。"""
    ckpt = torch.load(path, map_location=map_location or "cpu")
    model.load_state_dict(ckpt["model_state"])
    if optimizer is not None and "optimizer_state" in ckpt:
        optimizer.load_state_dict(ckpt["optimizer_state"])
    if scheduler is not None and "scheduler_state" in ckpt:
        scheduler.load_state_dict(ckpt["scheduler_state"])
    return ckpt

