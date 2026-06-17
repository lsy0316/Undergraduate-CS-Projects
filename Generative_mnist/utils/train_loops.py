"""通用训练/评估/保存函数。

包含：
- set_seed, get_device
- AE 训练/验证循环
- GAN / cGAN 训练循环（包含标签平滑、定期采样与保存）
"""
from __future__ import annotations
import os
import time
from typing import Dict, Tuple, Optional

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader

from pytorch_generative_mnist.config import Config
from pytorch_generative_mnist.utils.vis import save_grid, TBLogger, plot_losses, save_recon_comparison, plot_train_val_curves
from pytorch_generative_mnist.utils.checkpoint import save_checkpoint
from pytorch_generative_mnist.utils.metrics import fid_placeholder


def set_seed(seed: int):
    import random
    import numpy as np
    torch.manual_seed(seed)
    random.seed(seed)
    np.random.seed(seed)


def get_device(cfg: Config) -> torch.device:
    if cfg.device is not None:
        return torch.device(cfg.device)
    return torch.device("cuda" if torch.cuda.is_available() else "cpu")


def bce_targets(size: int, value: float, device: torch.device):
    return torch.full((size, 1), value, device=device)


def train_ae(cfg: Config, model: nn.Module, train_loader: DataLoader, val_loader: Optional[DataLoader] = None, resume_ckpt: Optional[str] = None):
    device = get_device(cfg)
    set_seed(cfg.seed)
    torch.backends.cudnn.benchmark = True

    model = model.to(device)
    # 根据配置选择损失
    if cfg.ae_loss.lower() == "bce":
        criterion = nn.BCEWithLogitsLoss()
        post_act = torch.sigmoid
        target_range = (0.0, 1.0)
    else:
        criterion = nn.MSELoss()
        post_act = torch.tanh
        target_range = (-1.0, 1.0)

    optimizer = optim.Adam(model.parameters(), lr=cfg.lr_g, betas=cfg.betas, weight_decay=cfg.weight_decay)

    tb = TBLogger(cfg.logs_dir, enable=cfg.use_tensorboard)

    best_val = float("inf")
    global_step = 0

    if resume_ckpt and os.path.isfile(resume_ckpt):
        ckpt = torch.load(resume_ckpt, map_location=device)
        model.load_state_dict(ckpt["model_state"])
        if "optimizer_state" in ckpt:
            optimizer.load_state_dict(ckpt["optimizer_state"])
        global_step = ckpt.get("global_step", 0)

    train_losses_per_epoch = []
    val_losses_per_epoch = []

    for epoch in range(1, cfg.epochs + 1):
        model.train()
        t0 = time.time()
        running_loss = 0.0
        n_batches = 0
        for i, (imgs, _) in enumerate(train_loader):
            imgs = imgs.to(device)
            if target_range == (0.0, 1.0):
                # 反归一化到 [0,1] 以匹配 BCEWithLogitsLoss 的目标
                targets = (imgs + 1) / 2
            else:
                targets = imgs

            optimizer.zero_grad()
            x_logits, z = model(imgs)
            preds = x_logits if target_range == (0.0, 1.0) else post_act(x_logits)
            loss = criterion(preds, targets)
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            n_batches += 1
            global_step += 1

            if i % 50 == 0:
                print(f"AE | epoch {epoch}/{cfg.epochs} iter {i}/{len(train_loader)} | AE_loss {loss.item():.4f} | lr {cfg.lr_g} | time {time.time()-t0:.1f}s | step {global_step}")
                tb.add_scalar("AE/loss", float(loss.item()), global_step)


        # 简单验证
        val_loss = None
        if val_loader is not None:
            model.eval()
            with torch.no_grad():
                losses = []
                for imgs, _ in val_loader:
                    imgs = imgs.to(device)
                    if target_range == (0.0, 1.0):
                        targets = (imgs + 1) / 2
                    else:
                        targets = imgs
                    x_logits, _ = model(imgs)
                    preds = x_logits if target_range == (0.0, 1.0) else post_act(x_logits)
                    l = criterion(preds, targets).item()
                    losses.append(l)
                val_loss = float(sum(losses) / len(losses))
                tb.add_scalar("AE/val_loss", val_loss, epoch)

        # 统计本 epoch 平均训练损失
        train_epoch_loss = (running_loss / max(1, n_batches)) if n_batches > 0 else float('nan')
        train_losses_per_epoch.append(train_epoch_loss)

        elapsed = time.time() - t0
        print(f"AE | epoch {epoch} done | train {train_epoch_loss:.4f} | val {val_loss if val_loss is not None else -1:.4f} | time {elapsed:.1f}s")

        # checkpoint
        is_best = (val_loss is not None and val_loss < best_val)
        if is_best:
            best_val = val_loss
        # 绘制按 epoch 的训练/验证曲线
        try:
            if val_loss is not None:
                val_losses_per_epoch.append(val_loss)
            plot_train_val_curves(train_losses_per_epoch, val_losses_per_epoch, os.path.join(cfg.logs_dir, "ae_train_val.png"), title="AE Train/Val Loss")
        except Exception:
            pass

        # 保存一次更直观的重构对比图（包含 epoch 与平均重建损失）
        try:
            with torch.no_grad():
                # 从当前 batch 随机取前 16 张
                n_vis = 16
                imgs_vis = imgs[:n_vis]
                x_logits_vis, _ = model(imgs_vis)
                preds_vis = post_act(x_logits_vis)
                title = f"epoch={epoch} | train_loss={train_epoch_loss:.4f} | val={val_loss if val_loss is not None else float('nan'):.4f}"
                save_recon_comparison(imgs_vis, preds_vis, os.path.join(cfg.samples_dir, f"ae_recon_epoch{epoch}.png"), title=title)
        except Exception:
            pass

        if (epoch % cfg.save_every_epoch == 0) or is_best:
            save_checkpoint({
                "epoch": epoch,
                "global_step": global_step,
                "model_state": model.state_dict(),
                "optimizer_state": optimizer.state_dict(),
                "best_val": best_val,
                "config": cfg.__dict__,
            }, cfg.ckpt_dir, prefix="ae", is_best=is_best, epoch=epoch)

    tb.close()


def train_gan(
    cfg: Config,
    G: nn.Module,
    D: nn.Module,
    train_loader: DataLoader,
    resume_ckpt: Optional[str] = None,
    conditional: bool = False,
):
    """训练 GAN / cGAN 的通用循环。

    参数:
        cfg: 全局配置（学习率、z_dim、日志/样本/ckpt 路径、是否标签平滑等）
        G: 生成器（conditional=True 时应接收 (z, y)）
    # 设备与随机性：尽可能固定随机种子、启用 cudnn.benchmark 加速卷积

        D: 判别器（conditional=True 时应接收 (x, y)）
        train_loader: 训练数据迭代器，返回 (images, labels)
    # 优化器：DCGAN 常用 Adam(lr=2e-4, betas=(0.5,0.999))，分别为 G 与 D 设置

    # 选择损失：对抗训练常用 BCEWithLogitsLoss，D 输出未过 sigmoid 的 logits

        resume_ckpt: 可选，恢复训练的 checkpoint 路径（包含 G/D/优化器状态）
        conditional: 是否为条件 GAN（cGAN）。True 时在 G/D 输入中注入标签。

    核心训练思路:
    - 判别器 D 的目标: 区分真实图像与生成图像。用 BCEWithLogitsLoss，真实标签为 1（可平滑到 0.9），假样本为 0。
    - 生成器 G 的目标: 迷惑判别器，让 D 将“生成样本”判为真（标签=1）。
    - 每个迭代中，先更新 D 再更新 G：
        1) 更新 D 时，使用 real 与 fake.detach() 计算两项损失并求和；对 fake 使用 detach() 断开 G 的梯度，避免影响 G。
        2) 更新 G 时，不再 detach，使得从 D 的输出对“假样本”为真的损失梯度能够回传到 G 的参数。
    - 标签平滑（可选）: 将真实标签从 1.0 调整为 0.9，减缓 D 过拟合/过自信，通常能带来更稳定的训练。
    - 条件注入（conditional=True）:
        * 生成器侧: 采用 (z, y) 作为输入以实现条件生成。
        * 判别器侧: 采用 (x, y) 输入，让 D 学会“类别一致性”的判别。

    返回:
        无。函数内部会负责日志记录、定期保存样图、绘制损失曲线、以及保存 last/best checkpoint。
    """

    device = get_device(cfg)
    set_seed(cfg.seed)
    torch.backends.cudnn.benchmark = True

    G = G.to(device)
    D = D.to(device)

    optG = optim.Adam(G.parameters(), lr=cfg.lr_g, betas=cfg.betas, weight_decay=cfg.weight_decay)
    optD = optim.Adam(D.parameters(), lr=cfg.lr_d, betas=cfg.betas, weight_decay=cfg.weight_decay)

    criterion = nn.BCEWithLogitsLoss()
    tb = TBLogger(cfg.logs_dir, enable=cfg.use_tensorboard)

    global_step = 0
    d_losses: list[float] = []
    g_losses: list[float] = []
    best_fid = float("inf")

    if resume_ckpt and os.path.isfile(resume_ckpt):
        ckpt = torch.load(resume_ckpt, map_location=device)
        G.load_state_dict(ckpt["G_state"]) ; D.load_state_dict(ckpt["D_state"])
        if "optG_state" in ckpt: optG.load_state_dict(ckpt["optG_state"]) ; optD.load_state_dict(ckpt["optD_state"])  # noqa
        global_step = ckpt.get("global_step", 0)

    for epoch in range(1, cfg.epochs + 1):
        t0 = time.time()
        for i, (real, labels) in enumerate(train_loader):
            real = real.to(device)
            labels = labels.to(device)
            bsz = real.size(0)

            # ----------------- Train D -----------------
            D.train(); G.train()
            optD.zero_grad()

            # real
            # 判别真实样本：目标靠近 1（可用 0.9 平滑），鼓励 D 输出大的正 logits

            real_logits = D(real) if not conditional else D(real, labels)
            real_label = 0.9 if cfg.smooth_labels else 1.0
            loss_real = criterion(real_logits, bce_targets(bsz, real_label, device))
            # 生成假样本并判别：目标为 0；对 fake 使用 detach()，避免本轮 D 的反向传播更新到 G


            # fake
            z = torch.randn(bsz, cfg.z_dim, device=device)
            if not conditional:
                fake = G(z)
                fake_logits = D(fake.detach())
            else:
            # 判别器总损失 = 真实样本损失 + 假样本损失

                fake = G(z, labels)
                fake_logits = D(fake.detach(), labels)
            loss_fake = criterion(fake_logits, bce_targets(bsz, 0.0, device))
            # 通过反向传播只更新 D 的参数


            loss_D = loss_real + loss_fake
            # ----------------- Train G -----------------
            # 更新生成器：希望 D 将 fake 判为真（目标=1），并允许梯度回传到 G

            loss_D.backward()
            optD.step()

            # ----------------- Train G -----------------
            optG.zero_grad()
            z = torch.randn(bsz, cfg.z_dim, device=device)
            # 重新采样 z 并生成假样本；不使用 detach，使梯度通过 D 回传到 G

            if not conditional:
                fake = G(z)
                logits = D(fake)
            else:
                fake = G(z, labels)
                logits = D(fake, labels)
            loss_G = criterion(logits, bce_targets(bsz, 1.0, device))
            loss_G.backward()
            optG.step()

            global_step += 1

            if i % 50 == 0:
                print(f"GAN{'-cond' if conditional else ''} | epoch {epoch}/{cfg.epochs} iter {i}/{len(train_loader)} | D_loss {loss_D.item():.4f} | G_loss {loss_G.item():.4f} | lrG {cfg.lr_g} | time {time.time()-t0:.1f}s")
                tb.add_scalar("GAN/D_loss", float(loss_D.item()), global_step)
                tb.add_scalar("GAN/G_loss", float(loss_G.item()), global_step)
                d_losses.append(float(loss_D.item()))
                g_losses.append(float(loss_G.item()))

            if global_step % cfg.sample_every_steps == 0:
                with torch.no_grad():
                    z = torch.randn(64, cfg.z_dim, device=device)
                    if not conditional:
                        samples = G(z)
                    else:
                        # 按模型的嵌入大小动态确定类别数，避免在 CelebA(2类) 下越界
                        try:
                            num_classes = int(getattr(getattr(G, 'embed', None), 'num_embeddings', 10))
                        except Exception:
                            num_classes = 10
                        # 尽量覆盖所有类别，重复采样到 64 张
                        reps = (64 + num_classes - 1) // num_classes
                        y = torch.arange(0, num_classes, device=device, dtype=torch.long).repeat_interleave(reps)[:64]
                        samples = G(z, y)
                    save_grid(samples, os.path.join(cfg.samples_dir, f"gan{'_cond' if conditional else ''}_epoch{epoch}_step{global_step}.png"), nrow=8)

        elapsed = time.time() - t0
        print(f"GAN{'-cond' if conditional else ''} | epoch {epoch} done | time {elapsed:.1f}s")

        # 保存一次 loss 曲线 PNG
        try:
            plot_losses({"D_loss": d_losses, "G_loss": g_losses}, os.path.join(cfg.logs_dir, f"{'cgan' if conditional else 'gan'}_loss.png"))
        except Exception:
            pass

        # 使用占位 FID 作为 best 判据（教学演示）：越小越好
        is_best = False
        try:
            G.eval()
            with torch.no_grad():
                real_batch, real_labels = next(iter(train_loader))
                real_batch = real_batch.to(device)
                real_labels = real_labels.to(device)
                b = min(real_batch.size(0), 64)
                z = torch.randn(b, cfg.z_dim, device=device)
                if not conditional:
                    fake_batch = G(z)
                else:
                    fake_batch = G(z, real_labels[:b])
                fid = fid_placeholder(fake_batch, real_batch[:b])
                if fid < best_fid:
                    best_fid = fid
                    is_best = True
        except Exception:
            pass

        if (epoch % cfg.save_every_epoch == 0) or is_best:
            save_checkpoint({
                "epoch": epoch,
                "global_step": global_step,
                "G_state": G.state_dict(),
                "D_state": D.state_dict(),
                "optG_state": optG.state_dict(),
                "optD_state": optD.state_dict(),
                "best_fid": best_fid,
                "config": cfg.__dict__,
            }, cfg.ckpt_dir, prefix=("cgan" if conditional else "gan"), is_best=is_best, epoch=epoch)

    tb.close()

