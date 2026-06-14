# 数据结构相关代码

## 📖 文件简介

本文件包含数据结构课程的核心实验代码，使用 C 语言实现了计算机科学中最基础、最重要的数据结构。代码涵盖了线性结构、树形结构、图结构以及常用的查找和排序算法。

---

## 📚 实现内容

### 1. 顺序表 (Sequential List)

顺序表是线性表的顺序存储结构，使用一段连续的存储单元依次存储线性表的数据元素。

**主要操作：**
- `InitList_Sq` - 初始化顺序表，分配初始内存空间
- `InputList_Sq` - 输入数据元素
- `InsertList_Sq` - 在指定位置插入元素
- `DeleteList_Sq` - 删除指定范围内的元素
- `Reverse` - 逆置顺序表元素
- `PrintList_Sq` - 打印顺序表内容

**核心代码：**
```c
typedef struct {
    ElemType *elem;      // 存储空间基址
    int length;          // 当前长度
    int listsize;        // 当前分配的存储容量
} SqList;
```

**时间复杂度：**
| 操作 | 时间复杂度 |
|-----|-----------|
| 访问元素 | O(1) |
| 插入元素 | O(n) |
| 删除元素 | O(n) |

---

### 2. 经典算法实现

#### 2.1 元素分区算法
将顺序表中所有小于 k 的元素放到左边，大于 k 的元素放到右边。

```c
void fun(SqList *L, int k) {
    int i = 0, j = L->length - 1, temp;
    while (i < j) {
        while (i < j && L->elem[i] < k) i++;
        while (i < j && L->elem[j] > k) j--;
        if (i < j) {
            temp = L->elem[i];
            L->elem[i] = L->elem[j];
            L->elem[j] = temp;
        }
    }
}
```

#### 2.2 查找并删除最大值
查找顺序表中的最大值，并删除所有等于最大值的元素。

```c
int findMaxList(SqList *L) {
    int i, max, k = 0;
    max = L->elem[0];
    // 查找最大值并删除
    for (i = 0; i < L->length; i++) {
        if (L->elem[i] > max) {
            max = L->elem[i];
        }
    }
    // 删除所有最大值元素
    for (i = 0; i < L->length; i++) {
        if (L->elem[i] == max)
            k++;
        else
            L->elem[i - k] = L->elem[i];
    }
    L->length -= k;
    return max;
}
```

#### 2.3 有序顺序表合并
将两个有序顺序表合并为一个新的有序顺序表。

```c
void MergeList(SqList *La, SqList *Lb, SqList *Lc) {
    int i = 0, j = 0, k = 0, x;
    while (i < La->length && j < Lb->length) {
        if (La->elem[i] <= Lb->elem[j]) {
            Lc->elem[k++] = La->elem[i++];
        } else {
            Lc->elem[k++] = Lb->elem[j++];
        }
    }
    // 处理剩余元素
    // ...
    Lc->length = k;
}
```

---

### 3. 集合运算

#### 3.1 集合并运算 (A∪B)
将两个集合合并，结果集合中不包含重复元素。

```c
void fun(SqList *La, SqList *Lb) {
    int j, i = 0;
    for (j = 0; j < Lb->length; j++) {
        if (!LocateList(La, Lb->elem[j])) {
            La->elem[La->length + i] = Lb->elem[j];
            i++;
        }
    }
    La->length += i;
}
```

#### 3.2 集合交运算 (A∩B)
求两个集合的交集。

```c
void fun(SqList *La, SqList *Lb) {
    int j, k = 0;
    for (j = 0; j < Lb->length; j++) {
        if (!LocateList(La, Lb->elem[j])) {
            k++;
        } else {
            Lb->elem[j - k] = Lb->elem[j];
        }
    }
    Lb->length -= k;
}
```

---

## 🔧 编译与运行

### 编译命令
```bash
gcc 数据结构相关代码.cpp -o data_structure
```

### 运行示例
```
输入：
5
1 2 3 4 5
3 100

输出：
1 2 100 3 4 5
```

---

## 📊 数据结构特点

| 数据结构 | 优点 | 缺点 |
|---------|------|------|
| 顺序表 | 随机访问O(1)，存储密度高 | 插入删除O(n)，需要预分配空间 |
| 链表 | 插入删除O(1)，动态分配 | 访问O(n)，需要额外指针空间 |

---

## 🎯 学习要点

1. **动态内存管理**：使用 `malloc` 和 `realloc` 实现动态扩容
2. **算法复杂度分析**：理解不同操作的时间复杂度
3. **边界条件处理**：正确处理空表、满表等特殊情况
4. **代码复用**：封装常用操作为函数，提高代码可维护性

---

## 📝 适用场景

- 学习数据结构基础概念
- 理解线性表的顺序存储实现
- 掌握基本的增删改查操作
- 算法设计与分析入门

---

## 🔗 相关资源

- 《数据结构（C语言版）》- 严蔚敏
- 《大话数据结构》- 程杰
- 《算法导论》- Thomas H. Cormen
