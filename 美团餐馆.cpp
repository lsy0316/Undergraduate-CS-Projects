#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHOP_NAME_LENGTH 500
#define MAX_ADDRESS_LENGTH 1000
#define FILE_BUFFER_SIZE 16384
#define HASH_TABLE_SIZE 50000
#define MAX_NODES 1000
#define INF INT_MAX

// 餐馆信息结构体
typedef struct {
    char shopType[MAX_SHOP_NAME_LENGTH];
    int shopId;
    char shopName[MAX_SHOP_NAME_LENGTH];
    char shopPassword[MAX_SHOP_NAME_LENGTH];
    double averageScore;
    double averagePrice;
    char address[MAX_ADDRESS_LENGTH];
    char phone[MAX_SHOP_NAME_LENGTH];
} RestaurantInfo;

// 哈希表节点结构体
typedef struct HashNode {
    RestaurantInfo restaurant;
    struct HashNode *next;
} HashNode;

// 哈希表结构体
typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
} HashTable;
typedef struct {
    int id;                     // 节点ID
    char name[100];             // 节点名称
} Node;

typedef struct {
    int numNodes;               // 节点数量
    int adjMatrix[MAX_NODES][MAX_NODES]; // 邻接矩阵
    Node nodes[MAX_NODES];      // 节点信息
} Graph;

// 初始化图
void initGraph(Graph *graph) {
    graph->numNodes = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph->adjMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// Dijkstra算法
void dijkstra(Graph *graph, int src, int dist[], int prev[]) {
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
                int alt = dist[u] + graph->adjMatrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
}

// 哈希函数
unsigned long hashFunction(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

// 将餐馆信息插入哈希表
void insertIntoHashTable(HashTable *hashTable, RestaurantInfo restaurant) {
    unsigned long index = hashFunction(restaurant.shopName);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newNode) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    *newNode = (HashNode){restaurant, hashTable->table[index]};
    hashTable->table[index] = newNode;
}

// 在哈希表中查找餐馆信息
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

// 从shop.txt文件读取餐馆信息并构建哈希表
void readShopFile(const char *filename, HashTable *hashTable) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开餐馆信息文件");
        exit(EXIT_FAILURE);
    }

    char buffer[FILE_BUFFER_SIZE];
    RestaurantInfo restaurant;
    while (fgets(buffer, FILE_BUFFER_SIZE, file)) {
        if (sscanf(buffer, "shoptype: %[^\n]", restaurant.shopType) != 1) continue;
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "shopId: %d", &restaurant.shopId);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "shopName: %[^\n]", restaurant.shopName);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "shopPassword: %[^\n]", restaurant.shopPassword);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "avgScore: %lf", &restaurant.averageScore);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "avePrice: %lf", &restaurant.averagePrice);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "address: %[^\n]", restaurant.address);
        fgets(buffer, FILE_BUFFER_SIZE, file);
        sscanf(buffer, "phone: %[^\n]", restaurant.phone);
        insertIntoHashTable(hashTable, restaurant);
    }
    fclose(file);
}

// 从distance.txt文件中查找某大学到指定商家的最短路径
double findShortestDistance(const char *filename, const char *shopName) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开距离信息文件");
        exit(EXIT_FAILURE);
    }

    double shortestDistance = -1;
    char buffer[FILE_BUFFER_SIZE];
    while (fgets(buffer, FILE_BUFFER_SIZE, file)) {
        char shopName1[MAX_SHOP_NAME_LENGTH], shopName2[MAX_SHOP_NAME_LENGTH];
        double distance;
        if (sscanf(buffer, "%*s %s %*s %s %lf", shopName1, shopName2, &distance) == 3) {
            if ((strcmp(shopName1, "某大学") == 0 && strcmp(shopName2, shopName) == 0) ||
                (strcmp(shopName2, "某大学") == 0 && strcmp(shopName1, shopName) == 0)) {
                if (shortestDistance == -1 || distance < shortestDistance) {
                    shortestDistance = distance;
                }
            }
        }
    }
    fclose(file);
    return shortestDistance;
}

int main() {
    HashTable hashTable = {0};

    // 从文件读取餐馆信息并构建哈希表
    readShopFile("D:\\Microsoft浏览器\\23级数据结构实验期末考试题题目3\\23级数据结构实验期末考试题题目3\\shop.txt", &hashTable);

    // 用户输入商家名称
    char shopName[MAX_SHOP_NAME_LENGTH];
    printf("请输入商家名称: ");
    fgets(shopName, MAX_SHOP_NAME_LENGTH, stdin);
    shopName[strcspn(shopName, "\n")] = '\0'; // 移除换行符

    // 查找餐馆
    HashNode *foundRestaurant = searchInHashTable(&hashTable, shopName);
    if (foundRestaurant) {
        RestaurantInfo *restaurant = &foundRestaurant->restaurant;
        printf("\n餐馆信息:\n");
        printf("餐馆类型: %s\n", restaurant->shopType);
        printf("商家ID: %d\n", restaurant->shopId);
        printf("商家名称: %s\n", restaurant->shopName);
        printf("平均评分: %.2lf\n", restaurant->averageScore);
        printf("平均价格: %.2lf\n", restaurant->averagePrice);
        printf("地址: %s\n", restaurant->address);
        printf("电话: %s\n", restaurant->phone);

        // 查找最短路径
        double shortestDistance = findShortestDistance("D:\\Microsoft浏览器\\23级数据结构实验期末考试题题目3\\23级数据结构实验期末考试题题目3\\distance.txt", shopName);
        if (shortestDistance != -1) {
            printf("某大学到该商家的最短路径: %.2lf 公里\n", shortestDistance);
        } else {
            printf("未找到某大学到该商家的距离信息\n");
        }
    } else {
        printf("\n餐馆不存在\n");
    }

    return 0;
}
//门框胡同百年卤煮（彰化路店）
//俏江南（腾达大厦店）

