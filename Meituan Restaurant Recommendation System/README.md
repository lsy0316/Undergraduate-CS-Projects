# 美团餐馆推荐系统

## 📖 项目简介

本项目是数据结构课程设计实践项目，模拟美团餐馆信息管理系统，实现了餐馆数据的高效存储、检索、路径规划和智能推荐功能。项目综合运用了哈希表、图、Dijkstra 最短路径算法等核心数据结构与算法知识。

---

## 🎯 项目目标

1. 掌握哈希表的设计与实现
2. 理解图的各种存储结构（邻接矩阵）
3. 实现经典最短路径算法（Dijkstra）
4. 综合运用数据结构解决实际问题
5. 实现智能推荐系统

---

## 🏗️ 系统架构

```
┌─────────────────────────────────────────────────────────────┐
│                     用户界面层                              │
│                 (控制台菜单系统)                             │
├─────────────────────────────────────────────────────────────┤
│                     业务逻辑层                              │
│    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐   │
│    │  哈希表检索  │    │  最短路径    │    │  智能推荐   │   │
│    └─────────────┘    └─────────────┘    └─────────────┘   │
├─────────────────────────────────────────────────────────────┤
│                     数据存储层                              │
│    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐   │
│    │  shop.txt   │    │  distance.txt│    │ restaurant  │   │
│    │ (餐馆信息)   │    │ (距离信息)   │    │ (数组缓存)   │   │
│    └─────────────┘    └─────────────┘    └─────────────┘   │
└─────────────────────────────────────────────────────────────┘
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
typedef struct HashNode {
    RestaurantInfo restaurant;    // 餐馆信息
    struct HashNode *next;        // 链地址法解决冲突
} HashNode;

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
    int id;                         // 节点ID
    char name[MAX_SHOP_NAME_LENGTH]; // 节点名称
} Node;

typedef struct {
    int numNodes;                   // 节点数量
    double adjMatrix[MAX_NODES][MAX_NODES]; // 邻接矩阵（浮点型）
    Node nodes[MAX_NODES];          // 节点信息
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

#### 内存释放

```c
void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = hashTable->table[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp);
        }
        hashTable->table[i] = NULL;
    }
}
```

**时间复杂度**：
- 平均情况：O(1)
- 最坏情况：O(n)（所有元素冲突）

### 2. Dijkstra 最短路径算法

```c
void dijkstra(Graph *graph, int src, double dist[], int prev[]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int i = 0; i < graph->numNodes; i++) {
        int u = -1;
        for (int j = 0; j < graph->numNodes; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (dist[u] == INF) break;
        visited[u] = 1;

        for (int v = 0; v < graph->numNodes; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] != INF) {
                double alt = dist[u] + graph->adjMatrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
}
```

#### 路径打印

```c
void printPath(Graph *graph, int prev[], int v) {
    if (prev[v] == -1) {
        printf("%s", graph->nodes[v].name);
        return;
    }
    printPath(graph, prev, prev[v]);
    printf(" -> %s", graph->nodes[v].name);
}
```

**算法步骤**：
1. 初始化：设置源点距离为0，其他为无穷大
2. 选择：从未访问节点中选择距离最小的节点
3. 更新：更新该节点所有邻接节点的距离
4. 重复：直到所有节点都被访问

### 3. 智能推荐功能

支持三种排序方式：
- **按评分排序**：优先推荐高评分餐馆
- **按价格排序**：优先推荐低价餐馆
- **按距离排序**：优先推荐最近餐馆

```c
if (strcmp(sortBy, "score") == 0) {
    // 冒泡排序：降序排列评分
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (restaurants[indices[i]].averageScore < restaurants[indices[j]].averageScore) {
                swap(indices[i], indices[j]);
            }
        }
    }
}
```

---

## 📁 数据文件格式

### shop.txt（餐馆信息）

```
shoptype: Chinese
shopId: 1
shopName: LaoWangHome
shopPassword: 123456
avgScore: 4.5
avePrice: 35.0
address: WenHua Road 123
phone: 13800138000
```

### distance.txt（距离信息）

```
University LaoWangHome 1.5
University ChuanWeiXuan 2.3
LaoWangHome ChuanWeiXuan 0.8
...
```

---

## 🚀 使用方法

### 编译

```bash
gcc restaurant.cpp -o restaurant -O2 -static
```

### 运行

```bash
./restaurant
```

### 程序菜单

```
+==========================================================+
|         Meituan Restaurant Recommendation System         |
+==========================================================+
|  1. Search Restaurant by Name                            |
|  2. List All Restaurants                                 |
|  3. Filter Restaurants by Type                           |
|  4. Recommend Restaurants                                |
|  5. Exit                                                 |
+==========================================================+
Enter your choice (1-5): 
```

### 示例交互

#### 1. 搜索餐馆

```
Enter your choice (1-5): 1
Enter restaurant name: LaoWangHome

Restaurant Information:
+---------------------+--------------------------+
| Field               | Value                    |
+---------------------+--------------------------+
| Type                | Chinese
| ID                  | 1
| Name                | LaoWangHome
| Average Score       | 4.50
| Average Price       | 35.00
| Address             | WenHua Road 123
| Phone               | 13800138000
+---------------------+--------------------------+

Shortest Path from University to LaoWangHome:
University -> LaoWangHome
Total Distance: 1.50 km
```

#### 2. 智能推荐

```
Enter your choice (1-5): 4
Sort by (score/price/distance): score

Top 5 Recommended Restaurants (sorted by score):
+-----+------------------+--------+--------+--------+--------+
| Rank| Name             | Type   | Score  | Price  | Distance|
+-----+------------------+--------+--------+--------+--------+
| 1   | ChongQingHotPot  | HotPot | 4.90   | 55.00  | 2.80 km |
| 2   | ChuanWeiXuan     | Sichuan| 4.80   | 42.00  | 2.30 km |
| 3   | YueGang          | Cantonese| 4.70 | 50.00  | 2.00 km |
| 4   | Vienna           | Western| 4.60   | 88.00  | 3.00 km |
| 5   | LaoWangHome      | Chinese| 4.50   | 35.00  | 1.50 km |
+-----+------------------+--------+--------+--------+--------+
```

---

## 📊 算法复杂度分析

| 操作 | 时间复杂度 | 空间复杂度 |
|-----|-----------|-----------|
| 哈希表插入 | O(1) 平均 | O(1) |
| 哈希表查找 | O(1) 平均 | O(1) |
| Dijkstra | O(V²) | O(V) |
| 文件读取 | O(n) | O(n) |
| 推荐排序 | O(n²) | O(n) |

---

## 🎨 项目特色

1. **高效检索**：使用哈希表实现 O(1) 时间复杂度的餐馆查找
2. **路径规划**：集成 Dijkstra 算法计算最优送餐路线，并打印完整路径
3. **链地址法**：有效解决哈希冲突问题
4. **智能推荐**：支持按评分、价格、距离三种方式排序推荐
5. **菜单系统**：完整的交互式控制台菜单，操作便捷
6. **类型筛选**：支持按餐馆类型筛选
7. **列表展示**：表格形式展示餐馆信息，美观清晰
8. **内存管理**：完善的内存释放机制，避免内存泄漏
9. **模块化设计**：各功能模块独立，便于维护和扩展

---

## 🔄 扩展方向

1. **优化哈希函数**：使用更高效的字符串哈希算法
2. **优先队列优化**：使用最小堆优化 Dijkstra 算法至 O(E log V)
3. **图形界面**：添加 Web 或 GUI 界面
4. **数据库集成**：使用 MySQL 存储数据
5. **推荐算法**：基于用户评分实现协同过滤推荐
6. **多条件筛选**：支持评分区间、价格区间等多条件组合筛选
7. **用户注册登录**：实现完整的用户系统
8. **订单管理**：添加订单创建和管理功能

---

## 📝 学习要点

1. **哈希表设计**：理解哈希函数设计和冲突解决方法
2. **图论算法**：掌握 Dijkstra 算法的原理和实现
3. **文件操作**：学习 C 语言文件读写
4. **结构体设计**：合理设计数据结构存储复杂信息
5. **排序算法**：实现基于不同关键字的排序功能
6. **内存管理**：理解动态内存分配和释放的重要性
7. **菜单系统设计**：设计友好的用户交互界面

---

## 🔗 相关资源

- 《数据结构（C语言版）》- 严蔚敏
- 《算法导论》- Thomas H. Cormen
- 《大话数据结构》- 程杰
