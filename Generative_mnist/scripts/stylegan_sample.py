#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
StyleGAN 预训练权重采样小工具

说明：
- 该脚本是对 NVLabs/stylegan2-ada-pytorch 中 generate.py 的轻量封装。
- 你需要先 clone 官方仓库并设置环境变量 STYLEGAN2_ADA_DIR 指向其根目录。

用法示例：
    python scripts/stylegan_sample.py \
        --pkl models/ffhq.pkl \
        --num 64 \
        --trunc 0.7 \
        --outdir experiments/samples_from_ffhq

等价于调用：
    python $STYLEGAN2_ADA_DIR/generate.py --outdir=... --trunc=0.7 --seeds=0-63 --network=models/ffhq.pkl
"""

from __future__ import annotations
import argparse
import os
import sys
import subprocess
from pathlib import Path


def parse_args():
    p = argparse.ArgumentParser(description="Sample images from a StyleGAN .pkl using NVLabs generate.py")
    p.add_argument("--pkl", required=True, type=str, help="Path to pretrained StyleGAN .pkl (e.g., ffhq.pkl)")
    p.add_argument("--num", type=int, default=64, help="Number of images to sample (default: 64)")
    p.add_argument("--trunc", type=float, default=0.7, help="Truncation psi (default: 0.7)")
    p.add_argument("--outdir", type=str, default="experiments/samples_from_pkl", help="Output directory")
    p.add_argument("--seeds", type=str, default="", help="Custom seeds string, e.g., 0,1,2,3 or 0-63 (overrides --num)")
    p.add_argument("--stylegan-dir", type=str, default=os.environ.get("STYLEGAN2_ADA_DIR", ""), help="Path to NVLabs/stylegan2-ada-pytorch (or set env STYLEGAN2_ADA_DIR)")
    return p.parse_args()


def main():
    args = parse_args()

    repo_dir = args.stylegan_dir
    gen_py = os.path.join(repo_dir, "generate.py")
    if not (repo_dir and os.path.isfile(gen_py)):
        raise SystemExit(
            "未找到 STYLEGAN2_ADA_DIR/generate.py。请先 clone 官方仓库并设置环境变量：\n"
            "  git clone https://github.com/NVlabs/stylegan2-ada-pytorch.git\n"
            "  export STYLEGAN2_ADA_DIR=/abs/path/to/stylegan2-ada-pytorch\n"
            "或使用 --stylegan-dir 显式指定路径。"
        )

    # 处理 seeds：优先使用 --seeds；否则根据 --num 生成 0-(num-1)
    if args.seeds:
        seeds_str = args.seeds
    else:
        seeds_str = f"0-{max(args.num - 1, 0)}"

    outdir = args.outdir
    Path(outdir).mkdir(parents=True, exist_ok=True)

    # 构建命令
    cmd = [
        sys.executable,
        gen_py,
        f"--outdir={outdir}",
        f"--trunc={args.trunc}",
        f"--seeds={seeds_str}",
        f"--network={args.pkl}",
    ]

    print("Launching:", " ".join(cmd))
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        raise SystemExit(f"generate.py 运行失败，退出码: {e.returncode}")

    print(f"采样完成，图片已保存到: {outdir}")


if __name__ == "__main__":
    main()

