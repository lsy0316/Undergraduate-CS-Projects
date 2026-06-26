# Cat and Dogs Classification

本项目使用 PyTorch 实现了基于迁移学习的猫狗二分类任务，对比了三种预训练模型（VGG16、ResNet18、ViT-B/16）在同一数据集上的性能表现。

## 环境要求

- Python >= 3.8
- PyTorch >= 1.8
- torchvision >= 0.9
- matplotlib

安装依赖：
```bash
pip install torch torchvision matplotlib
```

## 数据集

数据集包含猫和狗两类图片，存储在 `cat_dog/` 目录下：
```
cat_dog/
├── cat/    # 猫的图片
└── dog/    # 狗的图片
```

数据集已打包在 `cat_dog.zip` 中，使用前需解压：
```bash
unzip cat_dog.zip
```

## 模型对比

项目实现了三种经典模型的迁移学习：

| 模型 | 架构 | 预训练权重 |
|------|------|-----------|
| VGG16 | 卷积神经网络 | ImageNet |
| ResNet18 | 残差网络 | ImageNet |
| ViT-B/16 | Vision Transformer | ImageNet |

所有模型均采用"冻结特征提取层 + 微调分类头"的迁移学习策略。

## 数据增强

训练阶段使用以下数据增强策略：
- 随机水平翻转 (`RandomHorizontalFlip`)
- 随机旋转 10° (`RandomRotation`)
- 颜色抖动 (`ColorJitter`)
- 归一化（使用 ImageNet 均值和标准差）

## 训练设置

- 训练集/验证集比例：80% / 20%
- Batch Size：32
- 优化器：Adam
- 初始学习率：0.001
- 学习率调度器：StepLR（每 5 轮衰减 0.1）
- 训练轮数：10

## 使用方法

```bash
# 进入项目目录
cd "Cat and Dogs classification"

# 解压数据集（首次运行）
unzip cat_dog.zip

# 运行训练
python cnn_classfication.py
```

## 输出结果

训练完成后会生成：
- `best_vgg.pth` / `best_resnet.pth` / `best_vit.pth` - 各模型最佳权重
- `training_curves.png` - 训练损失和验证准确率曲线
- 终端打印各模型的性能对比表

## 典型输出

```
===== 开始训练 VGG =====
Epoch 1/10 | Loss: 0.5821 | Val Acc: 0.9120
Epoch 2/10 | Loss: 0.4156 | Val Acc: 0.9480
...
VGG 训练完成，耗时 120.50s，最佳验证准确率: 0.9750

==================================================
最终性能对比
==================================================
模型       最佳验证准确率          训练时间(10轮)
--------------------------------------------------
VGG        0.9750               120.50 s
RESNET     0.9820               95.30 s
VIT        0.9780               150.20 s
```

## 注意事项

- 代码会自动检测 GPU，优先使用 CUDA 加速
- 首次运行会下载预训练模型权重，需要网络连接
- 数据集路径固定为 `./cat_dog`，请确保解压后目录结构正确