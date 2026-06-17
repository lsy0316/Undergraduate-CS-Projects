"""全局配置与超参数。

提供默认值，并允许各训练/采样脚本通过 argparse 覆盖。
"""
from __future__ import annotations
import os
from dataclasses import dataclass


@dataclass
class Config:
    # 基本路径
    project_root: str = os.path.dirname(os.path.abspath(__file__))
    experiments_dir: str = os.path.join(project_root, "experiments")
    # 运行名称（模型名/实验名）：建议使用 "ae" | "gan" | "cgan"
    run_name: str = "ae"

    # 数据路径（共用）
    data_dir: str = os.path.join(project_root, "experiments", "data")

    # 结果路径（按模型名分目录）；若为 None 将在 __post_init__ 中根据 run_name 生成
    logs_dir: str | None = None
    samples_dir: str | None = None
    ckpt_dir: str | None = None

    # 训练通用
    seed: int = 42
    device: str | None = None  # 自动选择，脚本内解析
    batch_size: int = 128
    num_workers: int = max(2, os.cpu_count() // 2 if os.cpu_count() else 2)
    epochs: int = 20
    lr_g: float = 2e-4
    lr_d: float = 2e-4
    betas: tuple[float, float] = (0.5, 0.999)
    weight_decay: float = 0.0
    z_dim: int = 100
    img_size: int = 28
    img_channels: int = 1

    # AE
    ae_loss: str = "bce"  # 可选："bce" 或 "mse"
    ae_latent_dim: int = 64

    # GAN
    smooth_labels: bool = False  # 标签平滑
    sample_every_steps: int = 200  # 生成样本保存间隔（按 step）

    # 可视化/日志
    use_tensorboard: bool = True
    save_loss_png: bool = True
    save_every_epoch: int = 8  # 每多少个 epoch 保存一次 checkpoint

    def __post_init__(self):
        # 为日志/样本/权重构造按模型名区分的默认目录
        if self.logs_dir is None:
            self.logs_dir = os.path.join(self.experiments_dir, self.run_name, "logs")
        if self.samples_dir is None:
            self.samples_dir = os.path.join(self.experiments_dir, self.run_name, "samples")
        if self.ckpt_dir is None:
            self.ckpt_dir = os.path.join(self.experiments_dir, self.run_name, "checkpoints")


def ensure_dirs(cfg: Config) -> None:
    """确保必要目录存在。"""
    for d in [cfg.data_dir, cfg.logs_dir, cfg.samples_dir, cfg.ckpt_dir]:
        os.makedirs(d, exist_ok=True)

