from __future__ import annotations
"""训练条件 GAN（cGAN）。
用法：python scripts/train_cgan.py --epochs 50 --z-dim 100
"""
import argparse
import os
import sys

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

from pytorch_generative_mnist.config import Config, ensure_dirs
from pytorch_generative_mnist.data.mnist import get_dataloaders
from pytorch_generative_mnist.models.cgan import ConditionalGenerator, ConditionalDiscriminator
from pytorch_generative_mnist.utils.train_loops import train_gan


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--epochs", type=int, default=Config.epochs)
    p.add_argument("--batch-size", type=int, default=Config.batch_size)
    p.add_argument("--z-dim", type=int, default=Config.z_dim)
    p.add_argument("--lr-g", type=float, default=Config.lr_g)
    p.add_argument("--lr-d", type=float, default=Config.lr_d)
    p.add_argument("--smooth-labels", action="store_true")
    p.add_argument("--resume", type=str, default="")
    p.add_argument("--device", type=str, default=None)
    return p.parse_args()


def main():
    args = parse_args()
    cfg = Config(
        run_name="cgan",
        epochs=args.epochs,
        batch_size=args.batch_size,
        z_dim=args.z_dim,
        lr_g=args.lr_g,
        lr_d=args.lr_d,
        smooth_labels=bool(args.smooth_labels),
        device=args.device,
    )
    ensure_dirs(cfg)

    train_loader, _, _ = get_dataloaders(cfg)

    G = ConditionalGenerator(z_dim=cfg.z_dim)
    D = ConditionalDiscriminator()

    resume = args.resume if args.resume else None

    train_gan(cfg, G, D, train_loader, resume_ckpt=resume, conditional=True)


if __name__ == "__main__":
    main()

