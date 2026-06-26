# 🚀 Undergraduate-CS-Projects

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2FC%2B%2B-blue">
  <img src="https://img.shields.io/badge/Frontend-Vue3-green">
  <img src="https://img.shields.io/badge/Backend-Node.js-orange">
  <img src="https://img.shields.io/badge/Database-MySQL-red">
  <img src="https://img.shields.io/badge/Status-Updating-success">
</p>

## 📖 项目简介

本仓库记录了我本科阶段在人工智能专业学习过程中完成的部分课程代码、经典算法实现以及课程实践项目。

内容涵盖：

- 数据结构课程实验与代码实现
- 算法设计与分析课程实验
- C语言程序设计基础代码
- 经典算法实现与练习
- 数据库课程设计项目
- 前后端开发实践项目

通过持续整理与维护，希望构建一个可复现、可学习、可展示的技术成长档案。

---

## 📂 仓库结构

```text
Undergraduate-CS-Projects/
│
├── 数据结构相关代码.cpp
├── 算法设计与分析相关代码.cpp
├── 程序设计基础相关代码.cpp
├── 美团餐馆.cpp
├── Day1_递归算法.cpp
│
├── Meituan Restaurant Recommendation System/    # 美团餐馆推荐系统
├── college_management_system/                    # 智慧校园管理系统
├── book_management_system/                        # 图书后台管理系统
├── Generative_mnist/                              # MNIST生成模型
├── DeepEye-LungCancer-Screening/                  # 肺癌早筛系统
├── Cat and Dogs classification/                   # 猫狗分类项目
│
└── README.md
```

---

## 🧠 课程代码详解

### 📚 数据结构

本文件包含数据结构课程的核心实验代码，使用 C 语言实现，涵盖了计算机科学的基础数据结构。

**主要实现内容：**

| 数据结构 | 功能描述 |
|---------|---------|
| 顺序表 | 线性表的顺序存储结构，支持初始化、插入、删除、查找等操作 |
| 链表 | 单向链表的创建、遍历、节点插入与删除 |
| 栈 | 栈的顺序存储实现，包括入栈、出栈、栈顶元素获取 |
| 队列 | 循环队列的实现，支持入队、出队操作 |
| 二叉树 | 二叉树的构建、遍历（前序、中序、后序） |
| 图 | 图的邻接矩阵/邻接表存储，DFS/BFS遍历 |

**涉及经典算法：**

- **排序算法**：快速排序、归并排序、堆排序、冒泡排序
- **查找算法**：二分查找、顺序查找
- **图算法**：深度优先搜索（DFS）、广度优先搜索（BFS）、Dijkstra最短路径

**示例代码片段：**
```c
// 顺序表插入操作
void InsertList_Sq(SqList *L, int i, int e) {
    if (L->length >= L->listsize) {
        newbase = (ElemType*)realloc(L->elem, 
            (L->listsize + LISTINCREMENT) * sizeof(ElemType));
        L->elem = newbase;
        L->listsize += LISTINCREMENT;
    }
    for (j = L->length - 1; j >= i - 1; j--) {
        L->elem[j + 1] = L->elem[j];
    }
    L->elem[i - 1] = e;
    L->length++;
}
```

---

### 📖 算法设计与分析

本文件收录了算法设计与分析课程的经典算法实现，涵盖多种算法设计范式。

**主要内容：**

| 算法类型 | 代表性问题 |
|---------|-----------|
| 递归算法 | 斐波那契数列、数字各位分离、最大公约数/最小公倍数 |
| 分治算法 | 二分查找、归并排序、快速排序 |
| 动态规划 | 最长公共子序列、背包问题 |
| 贪心算法 | 活动安排问题、Huffman编码 |
| 回溯算法 | N皇后问题、图的遍历 |
| 分支限界法 | 最短路径搜索 |

**典型问题实现：**

1. **斐波那契数列**：递归与迭代两种实现方式
2. **最大公约数/最小公倍数**：辗转相除法（欧几里得算法）
3. **数字处理**：求整数各位数字、反转数字
4. **素数判定**：试除法判断素数

---

### 💻 程序设计基础

本文件是 C 语言程序设计的基础练习代码，适合初学者入门学习。

**练习内容：**

| 主题 | 知识点 |
|-----|-------|
| 指针基础 | 指针运算、指针与数组、指针作为函数参数 |
| 结构体 | 结构体定义、结构体数组、结构体指针 |
| 字符串处理 | 字符串操作、strcmp、strcpy 等函数使用 |
| 文件操作 | 文件读写、格式化输入输出 |
| 综合程序 | 排序算法（冒泡排序）、质因数分解 |

**典型例题：**
```c
// 指针作为函数参数
void print_value(int *ptr) {
    printf("%d", *ptr++);
    printf("%d", *ptr);
}

// 质因数分解
while (n != 1) {
    while (n % i == 0) {
        printf("*%d", i);
        n = n / i;
    }
    i++;
}
```

---

## 🚀 课程实践项目详解

### 🍽️ 美团餐馆推荐系统

**项目类型**：数据结构课程设计实践

**项目背景**：
仿美团餐馆信息管理系统，实现餐馆数据的高效存储与检索。

**核心功能模块**：

| 模块 | 功能描述 |
|-----|---------|
| 餐馆信息管理 | 餐馆名称、地址、类型、电话等信息存储 |
| 用户评分系统 | 餐馆评分管理、平均分计算 |
| 哈希表检索 | 使用哈希表实现餐馆信息快速查找 |
| 最短路径推荐 | 使用 Dijkstra 算法计算餐馆间最短距离 |

**技术实现**：

```c
// 哈希表节点结构
typedef struct HashNode {
    RestaurantInfo restaurant;
    struct HashNode *next;
} HashNode;

// Dijkstra最短路径算法
void dijkstra(Graph *graph, int src, int dist[], int prev[]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;
    // ... 算法实现
}
```

**项目特色**：
- 使用哈希表实现 O(1) 时间复杂度的餐馆信息查找
- 集成 Dijkstra 算法用于计算最优送餐路线
- 使用图的邻接矩阵存储餐馆间距离关系

---

### 🏫 智慧校园管理系统

**项目类型**：数据库原理课程设计

**项目概述**：
基于原生 HTML/CSS/JavaScript 开发的智慧校园信息管理系统，实现了校园内多种角色的信息化管理。

**系统角色与功能**：

| 角色 | 主要功能 |
|-----|---------|
| 学生 | 查看个人信息、课表、成绩、通知公告 |
| 教师 | 学生管理、成绩录入、课程安排 |
| 管理员 | 全面数据管理、统计分析、系统设置 |

**功能模块详解**：

| 模块 | 文件 | 功能描述 |
|-----|------|---------|
| 首页看板 | `index.html` | 数据统计展示、ECharts图表可视化 |
| 登录注册 | `login.html` / `register.html` | 用户身份认证 |
| 学生管理 | `student.html` | 学生信息增删改查 |
| 教师管理 | `teacher.html` | 教师信息管理 |
| 成绩管理 | `score.html` | 学生成绩录入与查询 |
| 课程管理 | `course.html` | 课程信息设置 |
| 通知公告 | `notice.html` | 校园通知发布与查看 |
| 系统设置 | `setting.html` | 系统参数配置 |

**技术栈**：

| 技术 | 用途 |
|-----|------|
| HTML5 | 页面结构 |
| CSS3 | 样式设计与布局 |
| JavaScript | 交互逻辑处理 |
| ECharts | 数据可视化图表 |
| Font Awesome | 图标库 |
| MySQL | 数据存储（后端） |

**系统特色**：
- 响应式布局设计，适配多种屏幕尺寸
- 使用 ECharts 实现数据统计图表展示
- 完整的权限管理机制
- Ajax 技术实现无刷新数据交互

---

### 📚 图书后台管理系统

**项目类型**：Web应用开发实践（前后端分离）

**项目概述**：
基于 React + TypeScript 构建的现代化图书管理系统，采用前后端分离架构，后端提供 RESTful API 服务。

**功能模块**：

| 模块 | 功能描述 |
|-----|---------|
| 图书列表 | 展示所有图书、支持分页显示 |
| 图书搜索 | 按书名、作者、ISBN 等条件搜索 |
| 图书新增 | 添加新图书信息 |
| 图书编辑 | 修改图书详细信息 |
| 图书删除 | 删除图书记录 |

**项目架构**：

```
book_management_system/
├── src/                      # React前端源码
│   ├── api/                  # API接口封装
│   │   └── books.ts          # 图书相关API
│   ├── components/           # React组件
│   │   ├── BookForm.tsx      # 图书表单组件
│   │   ├── BookList.tsx      # 图书列表组件
│   │   └── SearchBar.tsx     # 搜索栏组件
│   ├── data/                 # 静态数据
│   │   └── mockBooks.ts      # 模拟数据
│   ├── types/               # TypeScript类型定义
│   │   └── book.ts           # 图书类型
│   ├── App.tsx               # 根组件
│   ├── main.tsx              # 入口文件
│   └── index.css             # 全局样式
├── backend/                  # Express后端服务
│   ├── routes/               # 路由定义
│   │   └── books.ts          # 图书路由
│   ├── db.ts                 # 数据库连接
│   └── server.ts             # 服务器入口
├── public/                   # 静态资源
├── dist/                     # 打包输出目录
├── package.json              # 项目配置
├── vite.config.ts            # Vite配置
└── tsconfig.json             # TypeScript配置
```

**技术栈详情**：

| 技术 | 版本 | 作用 |
|-----|------|------|
| React | 18.2.0 | 前端UI框架 |
| TypeScript | 5.3.3 | 类型安全开发 |
| Vite | 5.0.12 | 快速构建工具 |
| Express | 5.2.1 | Node.js Web框架 |
| MySQL2 | 3.22.5 | MySQL数据库驱动 |
| TailwindCSS | 3.4.14 | 原子化CSS框架 |
| Axios | - | HTTP请求库 |

**API 接口设计**：

| 方法 | 路径 | 功能 |
|-----|------|------|
| GET | /api/books | 获取图书列表 |
| GET | /api/books/:id | 获取单本图书 |
| POST | /api/books | 新增图书 |
| PUT | /api/books/:id | 更新图书 |
| DELETE | /api/books/:id | 删除图书 |

**快速启动**：
```bash
# 安装依赖
npm install

# 启动后端服务
npm run server

# 启动前端开发服务器
npm run dev

# 同时启动前后端
npm run dev:all
```

**项目特色**：
- 前后端完全分离，各自独立部署
- 使用 TypeScript 确保代码类型安全
- 采用 Vite 实现毫秒级热更新
- 使用 TailwindCSS 实现快速样式开发
- 完整的图书 CRUD 操作
- 支持 CORS 跨域请求

---

### 🧠 MNIST 生成模型

**项目类型**：深度学习课程实践

**项目概述**：
基于 PyTorch 实现的 MNIST 数据集生成模型，包含 AutoEncoder (AE)、DCGAN 和条件生成对抗网络 (cGAN) 三种模型。

**核心功能**：

| 模型 | 功能描述 |
|-----|---------|
| AE | 自编码器，实现图像重构与特征压缩 |
| DCGAN | 深度卷积生成对抗网络，生成手写数字图像 |
| cGAN | 条件生成对抗网络，按类别生成指定数字 |

**技术栈**：
- PyTorch
- torchvision
- NumPy
- Matplotlib

**快速启动**：
```bash
pip install -r requirements.txt

# 训练 AE
python scripts/train_ae.py --epochs 20

# 训练 GAN
python scripts/train_gan.py --epochs 50 --z-dim 100

# 训练 cGAN
python scripts/train_cgan.py --epochs 50 --z-dim 100
```

---

### 🫁 深眸远虑——肺癌早筛系统

**项目类型**：医学影像 AI 系统

**项目概述**：
基于深度学习与计算机视觉技术的肺癌早期筛查系统，结合 PET-CT 医学影像数据，利用 Memba 模型对肺部异常区域进行智能分析。

**核心功能**：

| 功能 | 描述 |
|-----|------|
| 影像预处理 | PET-CT 数据读取、增强与标准化 |
| 病灶识别 | 基于 Memba 模型的风险区域检测 |
| 结果可视化 | 病灶区域标注与展示 |
| 模型推理 | 端到端推理流程 |

**技术栈**：
- Python
- PyTorch
- OpenCV
- SimpleITK
- NiBabel
- Flask / FastAPI

**应用场景**：
- 肺癌早期筛查
- 医学影像辅助诊断
- 医疗 AI 平台开发

---

### 🐱🐶 猫狗分类项目

**项目类型**：深度学习课程实践

**项目概述**：
基于 PyTorch 实现的猫狗二分类任务，对比了 VGG16、ResNet18、ViT-B/16 三种预训练模型的迁移学习效果。

**核心功能**：

| 模型 | 架构 | 预训练权重 |
|-----|------|-----------|
| VGG16 | 卷积神经网络 | ImageNet |
| ResNet18 | 残差网络 | ImageNet |
| ViT-B/16 | Vision Transformer | ImageNet |

**技术特点**：
- 采用"冻结特征提取层 + 微调分类头"的迁移学习策略
- 使用随机水平翻转、旋转、颜色抖动等数据增强
- 训练集/验证集比例：80% / 20%
- 自动生成训练曲线对比图

**快速启动**：
```bash
cd "Cat and Dogs classification"
unzip cat_dog.zip
python cnn_classfication.py
```

---

## 🛠 技术栈

### 编程语言

- C
- C++
- JavaScript
- TypeScript
- SQL
- Python

### 开发工具

- VS Code
- Git
- GitHub

### 前端开发

- Vue3
- React
- Vite
- TailwindCSS
- ECharts

### 后端开发

- Node.js
- Express
- Flask / FastAPI

### 数据库

- MySQL

### 机器学习 / 深度学习

- PyTorch
- torchvision
- NumPy
- OpenCV
- SimpleITK

---

## 📈 学习收获

通过本仓库中的课程实验与项目实践，我系统学习并掌握了：

- 数据结构与算法基础
- 程序设计思想
- 数据库设计与开发
- 前后端分离开发模式
- 项目需求分析与实现
- 软件工程实践能力

---

---

⭐ 如果本仓库对你有所帮助，欢迎 Star！
