from __future__ import annotations
"""统一训练脚本：支持 MNIST (28x28x1) 与 CelebA (64x64x3) 的 GAN/cGAN。

示例：
- MNIST GAN：python scripts/train_gan_unified.py --dataset mnist --model gan --epochs 50
- MNIST cGAN：python scripts/train_gan_unified.py --dataset mnist --model cgan --epochs 50 --z-dim 100
- CelebA GAN：python scripts/train_gan_unified.py --dataset celeba --model gan --epochs 50 --img-size 64
- CelebA cGAN(以 Smiling 属性为条件)：
  python scripts/train_gan_unified.py --dataset celeba --model cgan --epochs 50 --img-size 64 --attr Smiling
"""
import argparse
import os
import sys

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

import torch
import torch.nn as nn

from pytorch_generative_mnist.config import Config, ensure_dirs
from pytorch_generative_mnist.data.mnist import get_dataloaders as mnist_loaders
from pytorch_generative_mnist.data.celeba import get_dataloaders as celeba_loaders
from pytorch_generative_mnist.models.gan import Generator as G28, Discriminator as D28
from pytorch_generative_mnist.models.cgan import ConditionalGenerator as CG28, ConditionalDiscriminator as CD28
from pytorch_generative_mnist.models.gan64 import Generator64 as G64, Discriminator64 as D64
from pytorch_generative_mnist.models.cgan64 import ConditionalGenerator64 as CG64, ConditionalDiscriminator64 as CD64
from pytorch_generative_mnist.utils.train_loops import train_gan


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--dataset", type=str, choices=["mnist", "celeba"], required=True)
    p.add_argument("--model", type=str, choices=["gan", "cgan"], required=True)
    p.add_argument("--epochs", type=int, default=Config.epochs)
    p.add_argument("--batch-size", type=int, default=Config.batch_size)
    p.add_argument("--z-dim", type=int, default=Config.z_dim)
    p.add_argument("--lr-g", type=float, default=Config.lr_g)
    p.add_argument("--lr-d", type=float, default=Config.lr_d)
    p.add_argument("--smooth-labels", action="store_true")
    p.add_argument("--device", type=str, default=None)
    # CelebA 专用
    p.add_argument("--img-size", type=int, default=64)
    p.add_argument("--attr", type=str, default=None, help="CelebA 条件属性名，用于 cGAN 标签；如 Smiling")
    # 预训练权重初始化（非 resume）：仅加载 G/D 的权重，不恢复优化器/全局步数
    p.add_argument("--pretrained-G", type=str, default="", help="生成器预训练权重路径（.pth 或 state_dict）")
    p.add_argument("--pretrained-D", type=str, default="", help="判别器预训练权重路径（.pth 或 state_dict）")
    return p.parse_args()


def main():
    args = parse_args()
    run_name = f"{args.model}_{args.dataset}"
    cfg = Config(
        run_name=run_name,
        epochs=args.epochs,
        batch_size=args.batch_size,
        z_dim=args.z_dim,
        lr_g=args.lr_g,
        lr_d=args.lr_d,
        smooth_labels=bool(args.smooth_labels),
        device=args.device,
    )
    ensure_dirs(cfg)

    # 数据与模型选择
    # 可选：加载预训练初始化（仅加载权重，不恢复优化器状态）
    def _load_pretrained(module, path: str):
        if path and os.path.isfile(path):
            sd = torch.load(path, map_location="cpu")
            if isinstance(sd, dict) and all(k.startswith("module.") for k in sd.keys()):
                # 兼容 DataParallel 保存
                sd = {k[len("module."):]: v for k, v in sd.items()}
            try:
                module.load_state_dict(sd, strict=False)
                print(f"Loaded pretrained weights: {path}")
            except Exception as e:
                print(f"[WARN] Failed to load pretrained from {path}: {e}")

    conditional = (args.model == "cgan")

    if args.dataset == "mnist":
        train_loader, _, _ = mnist_loaders(cfg)
        if not conditional:
            G, D = G28(z_dim=cfg.z_dim), D28()
            _load_pretrained(G, args.pretrained_G)
            _load_pretrained(D, args.pretrained_D)
        else:
            G, D = CG28(z_dim=cfg.z_dim), CD28()
            _load_pretrained(G, args.pretrained_G)
            _load_pretrained(D, args.pretrained_D)
    else:
        train_loader, _, _ = celeba_loaders(cfg, img_size=args.img_size, target_attr=(args.attr if conditional else None))
        if not conditional:
            G, D = G64(z_dim=cfg.z_dim), D64()
            _load_pretrained(G, args.pretrained_G)
            _load_pretrained(D, args.pretrained_D)
        else:
            # CelebA cGAN 默认二分类属性（如 Smiling），标签空间为 {0,1}
            G, D = CG64(z_dim=cfg.z_dim, num_classes=2), CD64(num_classes=2)
            _load_pretrained(G, args.pretrained_G)
            _load_pretrained(D, args.pretrained_D)

    train_gan(cfg, G, D, train_loader, resume_ckpt=None, conditional=conditional)


if __name__ == "__main__":
    main()

