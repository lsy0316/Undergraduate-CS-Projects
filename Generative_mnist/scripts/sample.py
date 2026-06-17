from __future__ import annotations
"""采样脚本：从已训练的 GAN/cGAN 生成样本。

用法：
- GAN：python scripts/sample.py --mode gan --dataset mnist --num 64
- cGAN：python scripts/sample.py --mode cgan --dataset mnist --num 64 --class 7
"""
import argparse
import os
import sys

import torch

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

from pytorch_generative_mnist.config import Config, ensure_dirs
from pytorch_generative_mnist.models.gan import Generator
from pytorch_generative_mnist.models.cgan import ConditionalGenerator
from pytorch_generative_mnist.utils.vis import save_grid


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--mode", type=str, choices=["gan", "cgan"], required=True)
    p.add_argument("--dataset", type=str, choices=["mnist", "celeba"], default="mnist")
    p.add_argument("--num", type=int, default=64)
    p.add_argument("--z-dim", type=int, default=Config.z_dim)
    p.add_argument("--class", dest="klass", type=int, default=0, help="条件类别，仅 cGAN 有效 (MNIST:0-9, CelebA:0/1)")
    p.add_argument("--ckpt", type=str, default="")

    p.add_argument("--device", type=str, default=None)
    return p.parse_args()


def main():
    args = parse_args()

    # 根据 mode 设置 run_name，使读写目录定位到 experiments/<mode>/...
    cfg = Config(z_dim=args.z_dim, device=args.device, run_name=f"{args.mode}_{args.dataset}")
    ensure_dirs(cfg)

    device = torch.device(cfg.device or ("cuda" if torch.cuda.is_available() else "cpu"))

    # 根据数据集选择对应的生成器
    if args.dataset == "mnist":
        if args.mode == "gan":
            G = Generator(z_dim=cfg.z_dim).to(device)
            ckpt_path = args.ckpt or os.path.join(cfg.ckpt_dir, "gan_best.pth")
            ckpt = torch.load(ckpt_path, map_location=device)
            G.load_state_dict(ckpt["G_state"])
            G.eval()
            with torch.no_grad():
                z = torch.randn(args.num, cfg.z_dim, device=device)
                samples = G(z)
        else:
            G = ConditionalGenerator(z_dim=cfg.z_dim).to(device)
            ckpt_path = args.ckpt or os.path.join(cfg.ckpt_dir, "cgan_best.pth")
            ckpt = torch.load(ckpt_path, map_location=device)
            G.load_state_dict(ckpt["G_state"])
            G.eval()
            with torch.no_grad():
                z = torch.randn(args.num, cfg.z_dim, device=device)
                y = torch.full((args.num,), int(args.klass) % 10, device=device, dtype=torch.long)
                samples = G(z, y)
    else:
        from pytorch_generative_mnist.models.gan64 import Generator64 as G64
        from pytorch_generative_mnist.models.cgan64 import ConditionalGenerator64 as CG64
        if args.mode == "gan":
            G = G64(z_dim=cfg.z_dim).to(device)
            ckpt_path = args.ckpt or os.path.join(cfg.ckpt_dir, "gan_best.pth")
            ckpt = torch.load(ckpt_path, map_location=device)
            G.load_state_dict(ckpt["G_state"])
            G.eval()
            with torch.no_grad():
                z = torch.randn(args.num, cfg.z_dim, device=device)
                samples = G(z)
        else:
            G = CG64(z_dim=cfg.z_dim, num_classes=2).to(device)
            ckpt_path = args.ckpt or os.path.join(cfg.ckpt_dir, "cgan_best.pth")
            ckpt = torch.load(ckpt_path, map_location=device)
            G.load_state_dict(ckpt["G_state"])
            G.eval()
            with torch.no_grad():
                z = torch.randn(args.num, cfg.z_dim, device=device)
                # CelebA 条件标签默认为 {0,1}
                y = torch.full((args.num,), int(args.klass) % 2, device=device, dtype=torch.long)
                samples = G(z, y)

    out_path = os.path.join(cfg.samples_dir, f"sample_{args.mode}.png")
    save_grid(samples, out_path, nrow=int(args.num ** 0.5))
    print(f"Saved samples to {out_path}")


if __name__ == "__main__":
    main()

