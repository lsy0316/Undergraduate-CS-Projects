# 美团餐馆推荐系统

## 📖 项目简介

本项目是数据结构课程设计实践项目，模拟美团餐馆信息管理系统，实现了餐馆数据的高效存储、检索和路径规划功能。项目综合运用了哈希表、图、Dijkstra 最短路径算法等核心数据结构与算法知识。

---

## 🎯 项目目标

1. 掌握哈希表的设计与实现
2. 理解图的各种存储结构（邻接矩阵）
3. 实现经典最短路径算法（Dijkstra）
4. 综合运用数据结构解决实际问题

---

## 🏗️ 系统架构

```
┌─────────────────────────────────────────────────┐
│                   用户界面层                      │
│              (控制台输入/输出)                    │
├─────────────────────────────────────────────────┤
│                   业务逻辑层                      │
│    ┌─────────────┐    ┌─────────────────────┐   │
│    │  哈希表检索  │    │  最短路径计算       │   │
│    └─────────────┘    └─────────────────────┘   │
├─────────────────────────────────────────────────┤
│                   数据存储层                      │
│    ┌─────────────┐    ┌─────────────────────┐   │
│    │  shop.txt   │    │  distance.txt       │   │
│    │ (餐馆信息)   │    │ (距离信息)          │   │
│    └─────────────┘    └─────────────────────┘   │
└─────────────────────────────────────────────────┘
```

---

## 📊 数据结构设计

### 1. 餐馆信息结构体

```c
typedef struct {
    char shopType[MAX_SHOP_NAME_LENGTH];    // 餐馆类型
    int shopId;                              // 餐馆ID
    char shopName[MAX_SHOP_NAME_LENGTH];    // 餐馆名称
    char shopPassword[MAX_SHOP_NAME_LENGTH]; // 密码
    double averageScore;                     // 平均评分
    double averagePrice;                     // 平均价格
    char address[MAX_ADDRESS_LENGTH];        // 地址
    char phone[MAX_SHOP_NAME_LENGTH];        // 电话
} RestaurantInfo;
```

### 2. 哈希表结构

```c
// 哈希表节点
typedef struct HashNode {
    RestaurantInfo restaurant;    // 餐馆信息
    struct HashNode *next;        // 链地址法解决冲突
} HashNode;

// 哈希表
typedef struct {
    HashNode *table[HASH_TABLE_SIZE];  // 哈希表数组
} HashTable;
```

**哈希函数设计**：采用 DJB2 字符串哈希算法

```c
unsigned long hashFunction(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}
```

### 3. 图结构

```c
typedef struct {
    int id;                     // 节点ID
    char name[100];             // 节点名称
} Node;

typedef struct {
    int numNodes;               // 节点数量
    int adjMatrix[MAX_NODES][MAX_NODES]; // 邻接矩阵
    Node nodes[MAX_NODES];      // 节点信息
} Graph;
```

---

## 🔧 核心功能实现

### 1. 哈希表操作

#### 插入餐馆信息

```c
void insertIntoHashTable(HashTable *hashTable, RestaurantInfo restaurant) {
    unsigned long index = hashFunction(restaurant.shopName);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    *newNode = (HashNode){restaurant, hashTable->table[index]};
    hashTable->table[index] = newNode;
}
```

#### 查找餐馆信息

```c
HashNode *searchInHashTable(HashTable *hashTable, const char *shopName) {
    unsigned long index = hashFunction(shopName);
    HashNode *node = hashTable->table[index];
    while (node) {
        if (strcmp(node->restaurant.shopName, shopName) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}
```

**时间复杂度**：
- 平均情况：O(1)
- 最坏情况：O(n)（所有元素冲突）

### 2. Dijkstra 最短路径算法

```c
void dijkstra(Graph *graph, int src, int dist[], int prev[]) {
    int visited[MAX_NODES] = {0};
    
    // 初始化距离数组
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;
    
    // 主循环
    for (int i = 0; i < graph->numNodes; i++) {
        // 找到未访问的最短距离节点
        int u = -1;
        for (int j = 0; j < graph->numNodes; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (dist[u] == INF) break;
        visited[u] = 1;
        
        // 更新邻接节点距离
        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] != INF) {
                int alt = dist[u] + graph->adjMatrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
}
```

**算法步骤**：
1. 初始化：设置源点距离为0，其他为无穷大
2. 选择：从未访问节点中选择距离最小的节点
3. 更新：更新该节点所有邻接节点的距离
4. 重复：直到所有节点都被访问

---

## 📁 数据文件格式

### shop.txt（餐馆信息）

```
shoptype: 中餐
shopId: 1
shopName: 老王家常菜
shopPassword: 123456
avgScore: 4.5
avePrice: 35.0
address: 某某街道123号
phone: 13800138000
```

### distance.txt（距离信息）

```
某大学 老王家常菜 1.5
某大学 川菜馆 2.3
老王家常菜 川菜馆 0.8
...
```

---

## 🚀 使用方法

### 编译

```bash
gcc 美团餐馆.cpp -o restaurant
```

### 运行

```bash
./restaurant
```

### 示例交互

```
请输入餐馆名称: 老王家常菜

餐馆信息:
餐馆类型: 中餐
餐馆ID: 1
餐馆名称: 老王家常菜
平均评分: 4.50
平均价格: 35.00
地址: 某某街道123号
电话: 13800138000
某大学到餐馆的最短距离: 1.50 公里
```

---

## 📊 算法复杂度分析

| 操作 | 时间复杂度 | 空间复杂度 |
|-----|-----------|-----------|
| 哈希表插入 | O(1) 平均 | O(1) |
| 哈希表查找 | O(1) 平均 | O(1) |
| Dijkstra | O(V²) | O(V) |
| 文件读取 | O(n) | O(n) |

---

## 🎨 项目特色

1. **高效检索**：使用哈希表实现 O(1) 时间复杂度的餐馆查找
2. **路径规划**：集成 Dijkstra 算法计算最优送餐路线
3. **链地址法**：有效解决哈希冲突问题
4. **模块化设计**：各功能模块独立，便于维护和扩展

---

## 🔄 扩展方向

1. **优化哈希函数**：使用更高效的字符串哈希算法
2. **优先队列优化**：使用最小堆优化 Dijkstra 算法至 O(E log V)
3. **图形界面**：添加 Web 或 GUI 界面
4. **数据库集成**：使用 MySQL 存储数据
5. **推荐算法**：基于用户评分实现协同过滤推荐

---

## 📝 学习要点

1. **哈希表设计**：理解哈希函数设计和冲突解决方法
2. **图论算法**：掌握 Dijkstra 算法的原理和实现
3. **文件操作**：学习 C 语言文件读写
4. **结构体设计**：合理设计数据结构存储复杂信息

---

## 🔗 相关资源

- 《数据结构（C语言版）》- 严蔚敏
- 《算法导论》- Thomas H. Cormen
- 《大话数据结构》- 程杰
