from __future__ import annotations
"""训练 AE 脚本。
用法：python scripts/train_ae.py --epochs 20
"""
import argparse
import os
import sys

import torch

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

from pytorch_generative_mnist.config import Config, ensure_dirs
from pytorch_generative_mnist.data.mnist import get_dataloaders
from pytorch_generative_mnist.models.ae import AutoEncoder
from pytorch_generative_mnist.utils.train_loops import train_ae


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--epochs", type=int, default=Config.epochs)
    p.add_argument("--batch-size", type=int, default=Config.batch_size)
    p.add_argument("--ae-latent-dim", type=int, default=Config.ae_latent_dim)
    p.add_argument("--ae-loss", type=str, default=Config.ae_loss, choices=["bce", "mse"])
    p.add_argument("--lr", type=float, default=Config.lr_g)
    p.add_argument("--resume", type=str, default="")
    p.add_argument("--device", type=str, default=None)
    return p.parse_args()


def main():
    args = parse_args()
    cfg = Config(
        run_name="ae",
        epochs=args.epochs,
        batch_size=args.batch_size,
        ae_latent_dim=args.ae_latent_dim,
        ae_loss=args.ae_loss,
        lr_g=args.lr,
        device=args.device,
    )
    ensure_dirs(cfg)

    train_loader, val_loader, _ = get_dataloaders(cfg)

    model = AutoEncoder(latent_dim=cfg.ae_latent_dim)

    resume = args.resume if args.resume else None

    train_ae(cfg, model, train_loader, val_loader, resume_ckpt=resume)


if __name__ == "__main__":
    main()

