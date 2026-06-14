# 图书后台管理系统

## 📖 项目简介

图书后台管理系统是一个基于 React + TypeScript 构建的现代化 Web 应用，采用前后端分离架构。前端使用 React 18 + Vite + TailwindCSS，后端使用 Express + MySQL 提供 RESTful API 服务。本项目展示了现代 Web 开发的完整流程和最佳实践。

---

## 🎯 项目目标

1. 掌握 React 18 Hooks 开发模式
2. 学习 TypeScript 类型安全开发
3. 理解前后端分离架构
4. 实践 RESTful API 设计
5. 学习 TailwindCSS 原子化 CSS

---

## 🏗️ 系统架构

```
┌─────────────────────────────────────────────────────────┐
│                      前端 (React)                        │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐    │
│  │BookList │  │BookForm │  │SearchBar│  │  App    │    │
│  └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘    │
│       └────────────┴────────────┴────────────┘          │
│                         │                                │
│                    API Layer                             │
│                    (axios)                               │
└─────────────────────────┬───────────────────────────────┘
                          │ HTTP
                          ▼
┌─────────────────────────────────────────────────────────┐
│                    后端 (Express)                        │
│  ┌─────────────────────────────────────────────────┐   │
│  │              Routes / Controllers                 │   │
│  └─────────────────────────┬───────────────────────┘   │
│                            │                             │
│  ┌─────────────────────────▼───────────────────────┐   │
│  │              Database Layer (MySQL)              │   │
│  └─────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

---

## 📁 项目结构

```
book_management_system/
│
├── src/                          # 前端源码
│   ├── api/                      # API 接口封装
│   │   └── books.ts              # 图书相关 API
│   │
│   ├── components/               # React 组件
│   │   ├── BookForm.tsx          # 图书表单组件
│   │   ├── BookList.tsx          # 图书列表组件
│   │   └── SearchBar.tsx         # 搜索栏组件
│   │
│   ├── data/                     # 静态数据
│   │   └── mockBooks.ts          # 模拟数据
│   │
│   ├── types/                    # TypeScript 类型定义
│   │   └── book.ts               # 图书类型定义
│   │
│   ├── App.tsx                   # 根组件
│   ├── main.tsx                  # 应用入口
│   └── index.css                 # 全局样式
│
├── backend/                      # 后端服务
│   ├── routes/                   # 路由定义
│   │   └── books.ts              # 图书路由
│   ├── db.ts                     # 数据库连接配置
│   ├── server.ts                 # Express 服务器入口
│   └── createTable.sql           # 数据库建表脚本
│
├── public/                       # 静态资源
│   └── vite.svg                  # 网站图标
│
├── dist/                         # 打包输出目录
│   ├── assets/                   # 静态资源
│   └── index.html                # 入口 HTML
│
├── .env                          # 环境变量配置
├── package.json                  # 项目配置
├── vite.config.ts                # Vite 配置
├── tailwind.config.js            # TailwindCSS 配置
├── postcss.config.js             # PostCSS 配置
├── tsconfig.json                 # TypeScript 配置
└── tsconfig.node.json            # Node TypeScript 配置
```

---

## 🎨 功能模块

### 1. 图书列表展示

**组件**：`BookList.tsx`

**功能**：
- 展示所有图书信息
- 支持分页显示
- 显示图书封面、书名、作者、ISBN、价格等信息

**代码示例**：
```tsx
const BookList: React.FC<BookListProps> = ({ books, onEdit, onDelete }) => {
  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      {books.map((book) => (
        <div key={book.id} className="bg-white rounded-lg shadow p-4">
          <h3 className="text-lg font-semibold">{book.title}</h3>
          <p className="text-gray-600">作者: {book.author}</p>
          <p className="text-gray-600">ISBN: {book.isbn}</p>
          <p className="text-blue-600 font-bold">¥{book.price}</p>
          <div className="mt-4 flex gap-2">
            <button onClick={() => onEdit(book)}>编辑</button>
            <button onClick={() => onDelete(book.id)}>删除</button>
          </div>
        </div>
      ))}
    </div>
  );
};
```

---

### 2. 图书搜索

**组件**：`SearchBar.tsx`

**功能**：
- 按书名搜索
- 按作者搜索
- 按 ISBN 搜索
- 实时搜索结果

---

### 3. 图书新增/编辑

**组件**：`BookForm.tsx`

**功能**：
- 表单验证
- 新增图书
- 编辑图书信息

**表单字段**：
| 字段 | 类型 | 必填 | 说明 |
|-----|------|------|------|
| title | string | 是 | 书名 |
| author | string | 是 | 作者 |
| isbn | string | 是 | ISBN 编号 |
| price | number | 是 | 价格 |
| description | string | 否 | 简介 |

---

## 🔌 API 接口设计

### 接口列表

| 方法 | 路径 | 功能 | 请求体 | 响应 |
|-----|------|------|-------|------|
| GET | /api/books | 获取图书列表 | - | Book[] |
| GET | /api/books/:id | 获取单本图书 | - | Book |
| POST | /api/books | 新增图书 | Book | Book |
| PUT | /api/books/:id | 更新图书 | Book | Book |
| DELETE | /api/books/:id | 删除图书 | - | { message } |

### 接口示例

#### 获取图书列表
```http
GET /api/books HTTP/1.1
Host: localhost:3000
```

**响应**：
```json
[
  {
    "id": 1,
    "title": "JavaScript高级程序设计",
    "author": "Nicholas C. Zakas",
    "isbn": "978-7-115-27579-0",
    "price": 99.00,
    "description": "JavaScript经典著作"
  }
]
```

#### 新增图书
```http
POST /api/books HTTP/1.1
Host: localhost:3000
Content-Type: application/json

{
  "title": "React进阶之路",
  "author": "徐超",
  "isbn": "978-7-115-48273-1",
  "price": 79.00,
  "description": "React深入学习指南"
}
```

---

## 🛠️ 技术栈详解

### 前端技术

| 技术 | 版本 | 用途 |
|-----|------|------|
| React | 18.2.0 | UI 框架 |
| TypeScript | 5.3.3 | 类型安全 |
| Vite | 5.0.12 | 构建工具 |
| TailwindCSS | 3.4.14 | CSS 框架 |
| Axios | - | HTTP 请求 |

### 后端技术

| 技术 | 版本 | 用途 |
|-----|------|------|
| Express | 5.2.1 | Web 框架 |
| MySQL2 | 3.22.5 | 数据库驱动 |
| CORS | 2.8.6 | 跨域处理 |
| dotenv | 17.4.2 | 环境变量 |

---

## 📊 数据库设计

### 图书表 (books)

```sql
CREATE TABLE books (
    id INT PRIMARY KEY AUTO_INCREMENT,
    title VARCHAR(255) NOT NULL COMMENT '书名',
    author VARCHAR(100) NOT NULL COMMENT '作者',
    isbn VARCHAR(20) UNIQUE NOT NULL COMMENT 'ISBN编号',
    price DECIMAL(10, 2) NOT NULL COMMENT '价格',
    description TEXT COMMENT '简介',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
```

---

## 🚀 快速开始

### 1. 环境要求

- Node.js >= 18.0.0
- MySQL >= 8.0
- npm >= 9.0.0

### 2. 安装依赖

```bash
cd book_management_system
npm install
```

### 3. 配置数据库

```bash
# 1. 创建数据库
mysql -u root -p
CREATE DATABASE book_management;

# 2. 导入表结构
mysql -u root -p book_management < backend/createTable.sql
```

### 4. 配置环境变量

创建 `.env` 文件：
```env
DB_HOST=localhost
DB_PORT=3306
DB_USER=root
DB_PASSWORD=your_password
DB_DATABASE=book_management
PORT=3000
```

### 5. 启动项目

```bash
# 方式一：分别启动前后端
npm run server    # 启动后端服务 (端口 3000)
npm run dev       # 启动前端开发服务器 (端口 5173)

# 方式二：同时启动
npm run dev:all
```

### 6. 访问应用

- 前端地址：http://localhost:5173
- 后端 API：http://localhost:3000/api/books

---

## 📝 代码示例

### TypeScript 类型定义

```typescript
// src/types/book.ts
export interface Book {
  id: number;
  title: string;
  author: string;
  isbn: string;
  price: number;
  description?: string;
}

export interface BookFormData {
  title: string;
  author: string;
  isbn: string;
  price: number;
  description?: string;
}
```

### API 封装

```typescript
// src/api/books.ts
import axios from 'axios';
import { Book, BookFormData } from '../types/book';

const API_BASE_URL = 'http://localhost:3000/api';

export const bookApi = {
  getAll: async (): Promise<Book[]> => {
    const response = await axios.get(`${API_BASE_URL}/books`);
    return response.data;
  },

  getById: async (id: number): Promise<Book> => {
    const response = await axios.get(`${API_BASE_URL}/books/${id}`);
    return response.data;
  },

  create: async (data: BookFormData): Promise<Book> => {
    const response = await axios.post(`${API_BASE_URL}/books`, data);
    return response.data;
  },

  update: async (id: number, data: BookFormData): Promise<Book> => {
    const response = await axios.put(`${API_BASE_URL}/books/${id}`, data);
    return response.data;
  },

  delete: async (id: number): Promise<void> => {
    await axios.delete(`${API_BASE_URL}/books/${id}`);
  },
};
```

### Express 路由

```typescript
// backend/routes/books.ts
import express from 'express';
import { pool } from '../db';

const router = express.Router();

// 获取所有图书
router.get('/', async (req, res) => {
  try {
    const [rows] = await pool.query('SELECT * FROM books');
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: '获取图书列表失败' });
  }
});

// 新增图书
router.post('/', async (req, res) => {
  const { title, author, isbn, price, description } = req.body;
  try {
    const [result] = await pool.query(
      'INSERT INTO books (title, author, isbn, price, description) VALUES (?, ?, ?, ?, ?)',
      [title, author, isbn, price, description]
    );
    res.status(201).json({ id: result.insertId, ...req.body });
  } catch (error) {
    res.status(500).json({ error: '新增图书失败' });
  }
});

export default router;
```

---

## 🎯 项目特色

1. **类型安全**：全面使用 TypeScript，编译时发现错误
2. **快速开发**：Vite 提供毫秒级热更新
3. **原子化 CSS**：TailwindCSS 实现快速样式开发
4. **RESTful API**：标准化的接口设计
5. **前后端分离**：独立开发、独立部署

---

## 🔄 扩展方向

1. **用户认证**：添加 JWT 登录认证
2. **权限管理**：实现角色权限控制
3. **图片上传**：支持图书封面上传
4. **批量操作**：支持批量导入/导出
5. **单元测试**：添加 Jest 测试用例
6. **Docker 部署**：容器化部署

---

## 📚 学习要点

1. **React Hooks**：useState、useEffect、useCallback 等
2. **TypeScript**：接口定义、泛型、类型推断
3. **Express 中间件**：路由、错误处理、CORS
4. **MySQL 操作**：CRUD、事务、连接池
5. **Vite 配置**：代理、环境变量、构建优化

---

## 🔗 相关资源

- [React 官方文档](https://react.dev/)
- [TypeScript 官方文档](https://www.typescriptlang.org/)
- [Vite 官方文档](https://vitejs.dev/)
- [Express 官方文档](https://expressjs.com/)
- [TailwindCSS 官方文档](https://tailwindcss.com/)
