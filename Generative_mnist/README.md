本项目围绕 MNIST 数据集，使用 PyTorch 实现如下三类生成模型：

- AutoEncoder (AE)
- DCGAN 风格的 GAN
- 条件生成对抗网络 cGAN（以类别 y∈{0..9} 为条件）

## 环境与依赖

- Python ≥ 3.9
- PyTorch ≥ 2.0, torchvision ≥ 0.15（CPU/GPU 均可运行；若有 GPU 会自动加速）
- 其他依赖见 requirements.txt

安装依赖：

```bash
pip install -r requirements.txt
```

首次运行会自动下载 MNIST 数据集到本地。

## 项目结构

```text
pytorch_generative_mnist/
│── README.md
│── requirements.txt
│── config.py
│── data/
│   ├── mnist.py
│   └── celeba.py                 # 可选：CelebA 支持（img_size=64，属性条件标签）
│── models/
│   ├── ae.py
│   ├── gan.py
│   └── cgan.py
│── utils/
│   ├── train_loops.py
│   ├── metrics.py
│   ├── vis.py
│   └── checkpoint.py
│── scripts/
│   ├── train_ae.py
│   ├── train_gan.py
│   ├── train_cgan.py
│   └── sample.py
└── experiments/
    ├── logs/
    ├── samples/
    └── checkpoints/
```

说明：所有训练日志、图片与权重默认保存在 experiments/ 目录下，可在 config.py 中修改。

## 模型说明

- AE：轻量 CNN 编码器/解码器，输入 1×28×28，潜变量表示经过瓶颈层；重构损失可选 BCEWithLogitsLoss 或 MSELoss（在 config 中切换）。
- GAN（DCGAN 风格）：
  - 生成器：将潜变量 z∼N(0,1)（z_dim=100）上采样到 1×28×28，输出经 Tanh 映射到 [-1, 1]。
  - 判别器：卷积下采样到标量对真实/生成进行二分类（BCEWithLogitsLoss）。支持标签平滑（可选）。
- cGAN（条件版）：
  - 采用“标签嵌入后与 z 拼接”注入到生成器；判别器侧将标签嵌入映射为 28×28 特征图与图像在通道维拼接。

## 快速开始

- 训练 AE：

```bash
python scripts/train_ae.py --epochs 20
```

- 训练 GAN：

```bash
python scripts/train_gan.py --epochs 50 --z-dim 100
```

- 训练 cGAN：

```bash
python scripts/train_cgan.py --epochs 50 --z-dim 100
```

### CelebA（64x64x3）快速开始（GAN/cGAN）

- 统一脚本（可在 MNIST/CelebA 与 GAN/cGAN 间切换）：

```bash
# MNIST GAN / cGAN
python scripts/train_gan_unified.py --dataset mnist --model gan --epochs 50
python scripts/train_gan_unified.py --dataset mnist --model cgan --epochs 50 --z-dim 100

# CelebA GAN（img_size=64）
python scripts/train_gan_unified.py --dataset celeba --model gan --epochs 50 --img-size 64

# CelebA cGAN（以 Smiling 属性为条件）
python scripts/train_gan_unified.py --dataset celeba --model cgan --epochs 50 --img-size 64 --attr Smiling
```

- 采样：

```bash
# MNIST GAN 随机采样 64 张
python scripts/sample.py --mode gan --dataset mnist --num 64

# MNIST cGAN 条件采样 64 张，类别=7
python scripts/sample.py --mode cgan --dataset mnist --num 64 --class 7

# CelebA GAN 随机采样 64 张
python scripts/sample.py --mode gan --dataset celeba --num 64

# CelebA cGAN 条件采样 64 张，属性=1（如 Smiling）
python scripts/sample.py --mode cgan --dataset celeba --num 64 --class 1
```

- 恢复训练（任一脚本通用）：

```bash
python scripts/train_gan.py --resume experiments/checkpoints/gan_last.pth
```

所有脚本均支持命令行参数覆盖 config.py 中的默认设置（如 batch_size、z_dim、学习率等）。

## 可视化与日志

- 使用 torchvision.utils.make_grid 保存图片网格到 experiments/samples/
- 使用 matplotlib 保存 loss 曲线 PNG
- 可选 TensorBoard：若本机安装 tensorboard 将自动启用；否则自动降级为仅保存 PNG

### 预训练 StyleGAN 采样（可选）
- 需要先准备 NVLabs/stylegan2-ada-pytorch 的依赖（见 utils/stylegan.py 顶部说明）

### StyleGAN 微调（外部脚本）
- 前置：获取 NVLabs/stylegan2-ada-pytorch 源码并设置环境变量
```bash
git clone https://github.com/NVlabs/stylegan2-ada-pytorch.git
export STYLEGAN2_ADA_DIR=$(pwd)/stylegan2-ada-pytorch
```
- 检查 CelebA 数据目录是否到位（默认 experiments/data/celeba/img_align_celeba）
```bash
python scripts/check_celeba_data.py
```
- 基于预训练 .pkl 在 CelebA 上微调（示例）
```bash
python scripts/finetune_stylegan_celeba.py \
  --pkl ../model/ffhq.pkl \
  --kimg 200 \
  --gpus 1 \
  --batch 32 \
  --aug ada
```
- 微调结束后，脚本会尝试调用官方 generate.py 从最新快照生成一批样图（若 generate.py 存在）

- 使用 .pkl 进行采样：
```bash
python scripts/stylegan_sample.py --pkl ../model/ffhq.pkl --num 64 --trunc 0.7
```


## 常见问题与建议
- GAN 训练不稳定/崩塌：
  - 尝试降低学习率或使用标签平滑（config.smooth_labels）
  - 使用较大的 batch size（若显存允许）
  - 固定随机种子，观察可复现性
- AE 重构偏暗/偏亮：
  - 若使用 BCEWithLogitsLoss，确保输入在 [0,1]；使用 MSELoss 可适配 [-1,1] 或 [0,1]
- 初始化建议：使用 DCGAN 常用初始化或默认 kaiming/normal 即可

## 兼容性
- 以 PyTorch ≥ 2.0 / torchvision ≥ 0.15 / Python ≥ 3.9 为基线
- 若您的环境版本不同，建议参考 requirements.txt 进行适配

## 许可证
MIT

