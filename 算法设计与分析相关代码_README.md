# 算法设计与分析相关代码

## 📖 文件简介

本文件收录了算法设计与分析课程的经典算法实现，涵盖了多种算法设计范式，包括递归、分治、动态规划、贪心算法等。每个算法都配有详细的实现代码和注释说明。

---

## 📚 算法分类

### 1. 递归算法

递归是一种通过函数自身调用自身来解决问题的方法。

#### 1.1 斐波那契数列

```c
int fun(int n) {
    if (n <= 1)
        return n;
    return fun(n - 2) + fun(n - 1);
}
```

**时间复杂度**：O(2^n) - 存在大量重复计算

#### 1.2 输出整数的各位数字

```c
void fun(int n) {
    if (n == 0) return;
    else {
        printf("%d ", n % 10);
        fun(n / 10);
    }
}
```

**示例**：输入 `12345`，输出 `5 4 3 2 1`

#### 1.3 最大公约数与最小公倍数

使用辗转相除法（欧几里得算法）：

```c
int gcb(int m, int n) {
    if (m > n) swap(&m, &n);
    return Rgcb(m, n);
}

int Rgcb(int m, int n) {
    if (m == 0) return n;
    return Rgcb(n % m, m);
}
```

**最小公倍数**：`L = (m * n) / G`（G为最大公约数）

---

### 2. 分治算法

分治法的核心思想是"分而治之"：将大问题分解为小问题，分别解决后合并结果。

#### 2.1 求最大值和最小值

```c
void FindMaxMin(int a[], int i, int j, int *max, int *min) {
    int max1, min1;
    if (i == j)
        *max = *min = a[i];
    else if (i == j - 1) {
        *max = a[i] > a[j] ? a[i] : a[j];
        *min = a[i] < a[j] ? a[i] : a[j];
    } else {
        int m = (i + j) / 2;
        FindMaxMin(a, i, m, max, min);
        FindMaxMin(a, m + 1, j, &max1, &min1);
        if (*max < max1) *max = max1;
        if (*min > min1) *min = min1;
    }
}
```

**时间复杂度**：O(n)

#### 2.2 二分查找

```c
int FindX(int a[], int i, int j, int x) {
    if (i <= j) {
        int m = (i + j) / 2;
        if (x < a[m])
            return FindX(a, i, m - 1, x);
        else if (x > a[m])
            return FindX(a, m + 1, j, x);
        else
            return m;
    }
    return -1;
}
```

**时间复杂度**：O(log n)

---

### 3. 动态规划

动态规划通过将问题分解为重叠子问题，并存储子问题的解来避免重复计算。

#### 3.1 矩阵链乘法

求矩阵链乘法的最小乘法次数：

```c
int Max(vector<int> &p, int n) {
    int m[n][n], s[n][n];
    for (int i = 0; i < n; i++) {
        m[i][i] = 0;
    }
    for (int r = 2; r <= n; r++) {
        for (int i = 0; i <= n - r; i++) {
            int j = i + r - 1;
            m[i][j] = m[i + 1][j] + p[i] * p[i + 1] * p[j + 1];
            s[i][j] = i;
            for (int k = i + 1; k < j; k++) {
                int t = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (t < m[i][j]) {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
        }
    }
    return m[0][n - 1];
}
```

**时间复杂度**：O(n³)

#### 3.2 最长公共子序列 (LCS)

```c
int Max(int m, int n) {
    int c[m + 1][n + 1];
    int s[m + 1][n + 1];
    
    for (int i = 0; i <= m; i++) c[i][0] = 0;
    for (int j = 0; j <= n; j++) c[0][j] = 0;
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (x[i - 1] == y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                s[i][j] = 1;  // 对角线方向
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                s[i][j] = 2;  // 向上
            } else {
                c[i][j] = c[i][j - 1];
                s[i][j] = 3;  // 向左
            }
        }
    }
    return c[m][n];
}
```

**时间复杂度**：O(m × n)

---

### 4. 贪心算法

贪心算法在每一步选择中都采取当前状态下最优的选择。

#### 4.1 Dijkstra 最短路径算法

```c
void Dijkstra(int s, int d[], int path[], AMGraph *G) {
    bool inS[MAXV] = {false};
    for (int i = 0; i < G->vexnum; i++) {
        d[i] = G->arcs[s][i];
        if (i != s && d[i] < INF)
            path[i] = s;
        else
            path[i] = -1;
    }
    inS[s] = true;
    d[s] = 0;
    
    for (int i = 1; i < G->vexnum; i++) {
        int k = choose(d, inS, G->vexnum);
        if (k == -1) break;
        inS[k] = true;
        for (int j = 0; j < G->vexnum; j++) {
            if (!inS[j] && d[k] + G->arcs[k][j] < d[j]) {
                d[j] = d[k] + G->arcs[k][j];
                path[j] = k;
            }
        }
    }
}
```

**时间复杂度**：O(V²)

#### 4.2 多段图最短路径

使用动态规划求解多段图的最短路径：

```c
int DP(ALGraph G, int n, int m, int k, int *p) {
    int cost[n], d[n - 1];
    cost[n - 1] = 0;
    d[n - 1] = -1;
    
    for (int j = n - 2; j >= 0; j--) {
        int min = INT_MAX;
        for (ArcNode *r = G.vertices[j].firstarc; r; r = r->nextarc) {
            int v = r->adjvex;
            if (r->info + cost[v] < min) {
                min = r->info + cost[v];
                q = v;
            }
        }
        cost[j] = min;
        d[j] = q;
    }
    return cost[0];
}
```

---

### 5. 快速排序分区

```c
int Partion(vector<int> &a, int left, int right) {
    srand(time(NULL));
    int p = left + rand() % (right - left + 1);
    swap(a[left], a[p]);
    int pivot = a[left];
    int i = left, j;
    for (j = i + 1; j <= right; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i], a[left]);
    return i;
}
```

**时间复杂度**：
- 平均：O(n log n)
- 最坏：O(n²)

---

## 📊 算法复杂度对比

| 算法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|-----|-----------|-----------|---------|
| 斐波那契（递归） | O(2^n) | O(n) | 教学演示 |
| 斐波那契（DP） | O(n) | O(1) | 实际应用 |
| 二分查找 | O(log n) | O(1) | 有序数组查找 |
| 归并排序 | O(n log n) | O(n) | 稳定排序 |
| 快速排序 | O(n log n) | O(log n) | 通用排序 |
| Dijkstra | O(V²) | O(V) | 单源最短路径 |
| LCS | O(mn) | O(mn) | 序列比对 |

---

## 🔧 编译与运行

### 编译命令
```bash
# C语言编译
gcc 算法设计与分析相关代码.cpp -o algorithm

# C++编译（使用STL的部分）
g++ 算法设计与分析相关代码.cpp -o algorithm
```

---

## 🎯 学习要点

1. **递归思想**：理解递归的终止条件和递归调用
2. **分治策略**：分解 → 解决 → 合并
3. **动态规划**：状态定义、状态转移方程、边界条件
4. **贪心选择**：局部最优 → 全局最优
5. **复杂度分析**：时间复杂度和空间复杂度的权衡

---

## 📝 典型问题

### 最值问题
- 分治法求最大最小值
- 动态规划求最优解

### 查找问题
- 二分查找
- 第k小元素

### 路径问题
- 最短路径（Dijkstra）
- 多段图最短路径

### 序列问题
- 最长公共子序列
- 矩阵链乘法

---

## 🔗 相关资源

- 《算法导论》- Thomas H. Cormen
- 《算法设计与分析》- 王晓东
- 《挑战程序设计竞赛》- 秋叶拓哉
