from __future__ import annotations
"""在 CelebA 上基于预训练 StyleGAN2-ADA 权重进行微调的辅助脚本（调用 NVLabs 官方训练脚本）。

说明与前置条件：
- 本脚本不直接复现 StyleGAN2-ADA 的训练实现，而是以子进程方式调用 NVLabs/stylegan2-ada-pytorch 的 train.py。
- 请先获取官方仓库源码，并设置环境变量 STYLEGAN2_ADA_DIR 指向其根目录：

    git clone https://github.com/NVlabs/stylegan2-ada-pytorch.git
    export STYLEGAN2_ADA_DIR=/absolute/path/to/stylegan2-ada-pytorch

- 同时建议确保 CUDA/cuDNN、PyTorch 版本满足官方要求；CPU 上训练 StyleGAN 不现实。
- 预训练权重通常为 .pkl（例如官方 FFHQ 权重）。本脚本将以 --resume=<pkl> 方式继续训练（微调）。

数据路径：
- 本项目默认通过 torchvision 自动下载 CelebA 到 experiments/data/celeba 目录，图像位于 img_align_celeba 子目录。
- 若未找到该目录，可以通过 --data 指定自定义数据目录（包含若干图片文件，官方脚本会自动扫描）。

用法示例：
    # 基于 FFHQ 预训练权重，在 CelebA 上训练 1000 kimg（教学演示可调小，例如 50-200）
    python scripts/finetune_stylegan_celeba.py \
        --pkl /path/to/ffhq.pkl \
        --kimg 200 \
        --gpus 1 \
        --batch 32 \
        --aug ada
"""
import argparse
import os
import sys
import subprocess
from pathlib import Path

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

from pytorch_generative_mnist.config import Config


def default_celeba_img_dir(cfg: Config) -> str:
    # torchvision.datasets.CelebA 的默认展开路径：<root>/celeba/img_align_celeba
    cand = os.path.join(cfg.data_dir, "celeba", "img_align_celeba")
    return cand


def build_command(args, data_dir: str, outdir: str, train_py: str) -> list[str]:
    cmd = [sys.executable, train_py, f"--outdir={outdir}", f"--data={data_dir}"]
    # 训练基本选项
    if args.gpus:
        cmd.append(f"--gpus={args.gpus}")
    if args.batch:
        cmd.append(f"--batch={args.batch}")
    if args.kimg:
        cmd.append(f"--kimg={args.kimg}")
    if args.snap:
        cmd.append(f"--snap={args.snap}")
    if args.gamma:
        cmd.append(f"--gamma={args.gamma}")
    if args.cfg:
        cmd.append(f"--cfg={args.cfg}")
    if args.resume:
        cmd.append(f"--resume={args.resume}")
    if args.aug:
        cmd.append(f"--aug={args.aug}")
    if args.cudnn_benchmark:
        cmd.append("--cudnn-benchmark=1")
    return cmd


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--pkl", type=str, required=True, help="预训练 .pkl（作为 --resume）")
    p.add_argument("--data", type=str, default="", help="CelebA 图像目录（若留空将自动推断 experiments/data/celeba/img_align_celeba）")
    p.add_argument("--outdir", type=str, default="", help="输出目录（默认 experiments/stylegan_finetune）")
    # 训练选项（部分常用项）
    p.add_argument("--gpus", type=int, default=1)
    p.add_argument("--batch", type=int, default=32)
    p.add_argument("--kimg", type=int, default=200)
    p.add_argument("--snap", type=int, default=10)
    p.add_argument("--gamma", type=float, default=10.0)
    p.add_argument("--cfg", type=str, default="stylegan2", help="网络配置，参考官方：auto/stylegan2/stylegan3-t 等")
    p.add_argument("--aug", type=str, default="ada", help="数据增强策略：ada/noaug 等")
    p.add_argument("--cudnn-benchmark", action="store_true")
    return p.parse_args()


def main():
    args = parse_args()

    # StyleGAN2-ADA 官方仓库路径
    repo_dir = os.environ.get("STYLEGAN2_ADA_DIR", "")
    train_py = os.path.join(repo_dir, "train.py")
    if not (repo_dir and os.path.isfile(train_py)):
        raise SystemExit(
            "未找到 STYLEGAN2_ADA_DIR/train.py。请先 clone 官方仓库并设置环境变量：\n"
            "  git clone https://github.com/NVlabs/stylegan2-ada-pytorch.git\n"
            "  export STYLEGAN2_ADA_DIR=/abs/path/to/stylegan2-ada-pytorch\n"
        )

    cfg = Config(run_name="stylegan_finetune")

    # 数据目录推断
    data_dir = args.data or default_celeba_img_dir(cfg)
    if not os.path.isdir(data_dir):
        raise SystemExit(
            f"未找到 CelebA 图像目录: {data_dir}\n"
            "请使用 --data 指向包含图像文件的目录（官方脚本会递归扫描）。"
        )

    # 输出目录
    outdir = args.outdir or os.path.join(cfg.experiments_dir, "stylegan_finetune")
    os.makedirs(outdir, exist_ok=True)

    # 构建命令
    resume_path = args.pkl
    cmd = build_command(
        argparse.Namespace(
            gpus=args.gpus,
            batch=args.batch,
            kimg=args.kimg,
            snap=args.snap,
            gamma=args.gamma,
            cfg=args.cfg,
            resume=resume_path,
            aug=args.aug,
            cudnn_benchmark=args.cudnn_benchmark,
        ),
        data_dir=data_dir,
        outdir=outdir,
        train_py=train_py,
    )

    print("Launching:", " ".join(cmd))
    # 直接将控制台输出透传，便于用户观察官方训练日志
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        raise SystemExit(f"StyleGAN2-ADA 训练脚本返回非零退出码: {e.returncode}")

    # 微调结束后：尝试用官方 generate.py 采样若干样图
    gen_py = os.path.join(repo_dir, "generate.py")
    if os.path.isfile(gen_py):
        # 使用最新的网络快照（官方会在 outdir 下生成 network-snapshot-*.pkl）
        snaps = sorted([p for p in os.listdir(outdir) if p.startswith("network-snapshot-") and p.endswith('.pkl')])
        if snaps:
            latest = os.path.join(outdir, snaps[-1])
            gen_cmd = [sys.executable, gen_py, f"--outdir={os.path.join(outdir, 'samples')}", f"--trunc=0.7", f"--seeds=0-63", f"--network={latest}"]
            print("Sampling with:", " ".join(gen_cmd))
            try:
                subprocess.run(gen_cmd, check=True)
            except subprocess.CalledProcessError:
                print("[WARN] 采样失败，但微调过程已完成。")


if __name__ == "__main__":
    main()

