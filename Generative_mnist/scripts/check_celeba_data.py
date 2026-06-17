from __future__ import annotations
"""检查 CelebA 数据目录的小工具。

默认检查 experiments/data/celeba/img_align_celeba 是否存在并包含图像文件；
若未找到，打印提示与修复建议。
"""
import os
import sys
from glob import glob

# 允许从包根目录运行：将仓库根目录加入 sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..")))

from pytorch_generative_mnist.config import Config


def main():
    cfg = Config()
    default_dir = os.path.join(cfg.data_dir, "celeba", "img_align_celeba")
    print(f"默认 CelebA 目录: {default_dir}")
    if not os.path.isdir(default_dir):
        print("[WARN] 未找到默认目录。可能的原因：")
        print(" - 尚未下载 CelebA（可通过 data/celeba.py 首次加载自动下载，或手动准备数据）")
        print(" - 你使用了自定义数据路径。对于 StyleGAN 微调，请将 --data 指向包含 CelebA 图像的目录")
        sys.exit(1)

    img_glob = glob(os.path.join(default_dir, "*.jpg")) + glob(os.path.join(default_dir, "*.png"))
    if not img_glob:
        print(f"[WARN] 目录存在，但未发现图片文件: {default_dir}")
        sys.exit(2)

    print(f"[OK] 找到 {len(img_glob)} 个图像文件，路径看起来有效。")
    print("下一步：")
    print(" - 你可以运行 scripts/finetune_stylegan_celeba.py 进行微调；或")
    print(" - 使用 --data 指定自定义图像目录")


if __name__ == "__main__":
    main()

