# 程序设计基础相关代码

## 📖 文件简介

本文件是 C 语言程序设计的基础练习代码集合，涵盖了指针、结构体、字符串处理、数组操作、文件操作等核心知识点。代码以典型例题的形式呈现，适合初学者入门学习和巩固基础。

---

## 📚 内容分类

### 1. 指针基础

#### 1.1 指针与数组

```c
#include <stdio.h>
void print_value(int *ptr) {
    printf("%d", *ptr++);  // 先输出*ptr，然后ptr++
    printf("%d", *ptr);
}
int main() {
    int a[] = {1, 3, 5, 7};
    print_value(a);
    return 0;
}
// 输出结果：13
```

**知识点**：
- 指针作为函数参数传递
- 指针的自增运算
- 数组名作为指针使用

---

### 2. 结构体

#### 2.1 结构体定义与使用

```c
#include <string.h>
#include <stdio.h>

typedef struct student {
    char name[10];
    long no;
    float score;
} STU;

int main() {
    STU a = {"zhangsuan", 12001, 95},
        b = {"Zhangxian", 12002, 90},
        c = {"Zhanghuan", 12003, 95};
    STU d, *p = &d;
    
    d = a;
    if (strcmp(a.name, b.name) > 0)
        d = b;
    if (strcmp(c.name, d.name) > 0)
        d = c;
    
    printf("%ld%s\n", d.no, p->name);
    return 0;
}
// 输出结果：12002Zhangxian
```

**知识点**：
- 结构体类型定义（typedef）
- 结构体变量初始化
- 结构体指针与 `->` 运算符
- 字符串比较函数 `strcmp`

---

### 3. 排序算法

#### 3.1 冒泡排序

```c
// 冒泡排序
for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - 1 - i; j++)
        if (a[j] > a[j + 1]) {
            t = a[j];
            a[j] = a[j + 1];
            a[j + 1] = t;
        }
```

**时间复杂度**：O(n²)

**排序过程示意**：
```
原始序列：5 3 8 1 2
第1趟：  3 5 1 2 8
第2趟：  3 1 2 5 8
第3趟：  1 2 3 5 8
第4趟：  1 2 3 5 8
```

---

### 4. 数学问题

#### 4.1 质因数分解

```c
#include <stdio.h>
int main() {
    int n, i = 2;
    scanf("%d", &n);
    printf("%d=1", n);
    while (n != 1) {
        while (n % i == 0) {
            printf("*%d", i);
            n = n / i;
        }
        i++;
    }
    return 0;
}
// 输入：18
// 输出：18=1*2*3*3
```

#### 4.2 素数判断

```c
#include <stdio.h>
int prime(int a) {
    int i;
    for (i = 2; i <= a; i++)
        if (a % i == 0) break;
    if (a == i)
        return 1;
    else
        return 0;
}
int main() {
    int m, n, i;
    scanf("%d%d", &m, &n);
    for (i = m; i <= n; i++)
        if (prime(i) == 1)
            printf("%d\n", i);
    return 0;
}
```

---

### 5. 字符处理

#### 5.1 字符统计（桶排序思想）

```c
#include <stdio.h>
int main() {
    int a[1000] = {0}, i;
    char ch;
    scanf("%c", &ch);
    while (ch != '#') {
        if (ch >= 'A' && ch <= 'Z')
            ch = ch + 32;  // 大写转小写
        if (ch >= 'a' && ch <= 'z')
            a[ch - 'a']++;
        scanf("%c", &ch);
    }
    for (i = 0; i < 1000; i++)
        if (a[i] != 0)
            printf("%c:%d\n", i + 'a', a[i]);
    return 0;
}
```

#### 5.2 字符串逆序

```c
#include <stdio.h>
#include <string.h>
int main() {
    char s[100], *p, *q, t;
    int n;
    gets(s);
    n = strlen(s);
    p = s;
    q = s + n - 1;
    while (p <= q) {
        t = *p;
        *p = *q;
        *q = t;
        p++;
        q--;
    }
    puts(s);
    return 0;
}
```

#### 5.3 回文字符串判断

```c
int fun(char *str) {
    int i, j = strlen(str);
    for (i = 0; i < j / 2; i++)
        if (str[i] != str[j - i - 1])
            return 0;
    return 1;
}
```

---

### 6. 图形打印

#### 6.1 打印三角形

```c
#include <stdio.h>
int main() {
    int i, j, m, n;
    char ch;
    scanf("%c%d", &ch, &m);
    n = (m + 1) / 2;
    for (i = 1; i <= n; i++) {
        for (j = 0; j < n - i; j++)
            printf(" ");
        for (j = 0; j < 2 * i - 1; j++)
            putchar(ch);
        printf("\n");
    }
    return 0;
}
// 输入：* 5
// 输出：
//   *
//  ***
// *****
```

#### 6.2 九九乘法表

```c
#include <stdio.h>
int main() {
    int i, j, result;
    for (i = 1; i < 10; i++) {
        for (j = 1; j < 10; j++) {
            result = i * j;
            printf("%d*%d=%-3d", i, j, result);
        }
        printf("\n");
    }
    return 0;
}
```

#### 6.3 杨辉三角

```c
#include <stdio.h>
int main() {
    int i, j, n, a[100][100] = {0};
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            if (j == 0 || j == i)
                a[i][j] = 1;
            else
                a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
        }
    }
    // 打印杨辉三角
    for (i = 0; i < n; i++) {
        for (int k = 1; k < n - i; k++)
            printf("  ");
        for (j = 0; j <= i; j++)
            printf("%-4d", a[i][j]);
        printf("\n");
    }
    return 0;
}
```

#### 6.4 菱形打印

```c
int main() {
    int m, n, i, j, k;
    char ch;
    scanf("%d%c", &m, &ch);
    n = m / 2 + 1;
    // 上半部分
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n - i; j++)
            printf(" ");
        for (k = 1; k < 2 * i; k++) {
            printf("%c", ch);
            if (k > 1 && k < 2 * i - 1)
                printf("\b ");
        }
        printf("\n");
    }
    // 下半部分
    for (i = n - 1; i >= 1; i--) {
        // 类似上半部分
    }
    return 0;
}
```

---

### 7. 数值计算

#### 7.1 斐波那契数列

```c
#include <stdio.h>
int main() {
    int a, b, c, s, i, n;
    scanf("%d", &n);
    a = b = 1;
    s = 2;
    printf("1 1 ");
    for (i = 3; i <= n; i++) {
        c = a + b;
        printf("%d ", c);
        s += c;
        a = b;
        b = c;
    }
    printf("%d", s);  // 输出前n项和
    return 0;
}
```

#### 7.2 最小公倍数

```c
#include <stdio.h>
#define N 105

int gcd(int x, int y) {
    if (x % y == 0)
        return y;
    else
        return gcd(y, x % y);
}

int lcm(int x, int y) {
    return x * y / gcd(x, y);
}

int nlcm(int r[], int n) {
    if (n == 2)
        return lcm(r[0], r[1]);
    return lcm(r[0], nlcm(r + 1, n - 1));
}

int main() {
    int n, i, a[N];
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);
    printf("%d", nlcm(a, n));
    return 0;
}
```

---

### 8. 几何计算

#### 8.1 判断点与三角形的位置关系

```c
#include <stdio.h>
#include <math.h>

typedef struct point {
    double x, y;
} Point;

double dist(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                (p1.y - p2.y) * (p1.y - p2.y));
}

double area(Point p1, Point p2, Point p3) {
    double a, b, c, s;
    a = dist(p1, p2);
    b = dist(p1, p3);
    c = dist(p2, p3);
    s = (a + b + c) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

int main() {
    Point O, A, B, C;
    double s0, s1, s2, s3;
    scanf("%lf%lf%lf%lf%lf%lf%lf%lf", 
          &O.x, &O.y, &A.x, &A.y, &B.x, &B.y, &C.x, &C.y);
    s0 = area(A, B, C);
    s1 = area(A, B, O);
    s2 = area(A, C, O);
    s3 = area(B, C, O);
    if (s1 + s2 + s3 > s0)
        printf("Out");
    else if (s1 * s2 * s3 == 0)
        printf("On");
    else
        printf("In");
    return 0;
}
```

---

### 9. 进制转换

#### 9.1 十进制转 N 进制

```c
#include <stdio.h>
void xchg(int x, int n) {
    if (x) {
        xchg(x / n, n);
        printf("%d", x % n);
    }
}

int main() {
    int x, n, i;
    scanf("%d", &n);
    for (int i = 0; i < 5; i++) {
        scanf("%d", &x);
        if (x == 0)
            putchar('0');
        else
            xchg(x, n);
        if (i < 4)
            putchar('\n');
    }
    return 0;
}
```

---

## 📊 知识点总结

| 章节 | 知识点 | 难度 |
|-----|-------|------|
| 指针 | 指针运算、指针与数组 | ⭐⭐ |
| 结构体 | 结构体定义、结构体指针 | ⭐⭐ |
| 排序 | 冒泡排序 | ⭐⭐ |
| 数学 | 素数、质因数分解 | ⭐⭐ |
| 字符 | 字符串处理、字符统计 | ⭐⭐ |
| 图形 | 循环打印各种图形 | ⭐⭐⭐ |
| 数值 | 斐波那契、最小公倍数 | ⭐⭐ |
| 几何 | 点与图形位置关系 | ⭐⭐⭐ |
| 进制 | 进制转换 | ⭐⭐ |

---

## 🔧 编译与运行

```bash
gcc 程序设计基础相关代码.cpp -o basic
./basic
```

---

## 🎯 学习建议

1. **理解指针**：指针是 C 语言的核心，务必深入理解
2. **多动手实践**：修改代码参数，观察输出变化
3. **画图理解**：对于图形打印问题，先画出规律
4. **调试技巧**：使用 printf 输出中间结果进行调试

---

## 🔗 相关资源

- 《C程序设计》- 谭浩强
- 《C Primer Plus》- Stephen Prata
- 《C语言程序设计现代方法》- K.N. King
