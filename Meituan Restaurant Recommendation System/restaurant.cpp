#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_SHOP_NAME_LENGTH 500
#define MAX_ADDRESS_LENGTH 1000
#define FILE_BUFFER_SIZE 16384
#define HASH_TABLE_SIZE 50000
#define MAX_NODES 100
#define MAX_RESTAURANTS 1000
#define INF INT_MAX

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

typedef struct HashNode {
    RestaurantInfo restaurant;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
} HashTable;

typedef struct {
    int id;
    char name[MAX_SHOP_NAME_LENGTH];
} Node;

typedef struct {
    int numNodes;
    double adjMatrix[MAX_NODES][MAX_NODES];
    Node nodes[MAX_NODES];
} Graph;

void initGraph(Graph *graph) {
    graph->numNodes = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph->adjMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int findNodeIndex(Graph *graph, const char *name) {
    for (int i = 0; i < graph->numNodes; i++) {
        if (strcmp(graph->nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int addNode(Graph *graph, const char *name) {
    int index = findNodeIndex(graph, name);
    if (index != -1) return index;
    
    if (graph->numNodes >= MAX_NODES) {
        printf("Error: Max nodes exceeded!\n");
        return -1;
    }
    
    strcpy(graph->nodes[graph->numNodes].name, name);
    graph->nodes[graph->numNodes].id = graph->numNodes;
    return graph->numNodes++;
}

void addEdge(Graph *graph, const char *from, const char *to, double weight) {
    int u = addNode(graph, from);
    int v = addNode(graph, to);
    if (u != -1 && v != -1) {
        graph->adjMatrix[u][v] = weight;
        graph->adjMatrix[v][u] = weight;
    }
}

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

void printPath(Graph *graph, int prev[], int v) {
    if (prev[v] == -1) {
        printf("%s", graph->nodes[v].name);
        return;
    }
    printPath(graph, prev, prev[v]);
    printf(" -> %s", graph->nodes[v].name);
}

unsigned long hashFunction(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

void insertIntoHashTable(HashTable *hashTable, RestaurantInfo restaurant) {
    unsigned long index = hashFunction(restaurant.shopName);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    *newNode = (HashNode){restaurant, hashTable->table[index]};
    hashTable->table[index] = newNode;
}

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

void readShopFile(const char *filename, HashTable *hashTable, RestaurantInfo restaurants[], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open shop file");
        exit(EXIT_FAILURE);
    }

    char buffer[FILE_BUFFER_SIZE];
    RestaurantInfo restaurant;
    *count = 0;
    
    while (fgets(buffer, FILE_BUFFER_SIZE, file)) {
        if (strncmp(buffer, "shoptype:", 9) == 0) {
            sscanf(buffer + 9, " %[^\n]", restaurant.shopType);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 7, " %d", &restaurant.shopId);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 9, " %[^\n]", restaurant.shopName);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 14, " %[^\n]", restaurant.shopPassword);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 9, " %lf", &restaurant.averageScore);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 9, " %lf", &restaurant.averagePrice);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 8, " %[^\n]", restaurant.address);
            fgets(buffer, FILE_BUFFER_SIZE, file);
            sscanf(buffer + 6, " %[^\n]", restaurant.phone);
            
            insertIntoHashTable(hashTable, restaurant);
            restaurants[(*count)++] = restaurant;
        }
    }
    fclose(file);
}

void readDistanceFile(const char *filename, Graph *graph) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open distance file");
        exit(EXIT_FAILURE);
    }

    char buffer[FILE_BUFFER_SIZE];
    while (fgets(buffer, FILE_BUFFER_SIZE, file)) {
        char node1[MAX_SHOP_NAME_LENGTH], node2[MAX_SHOP_NAME_LENGTH];
        double distance;
        if (sscanf(buffer, "%s %s %lf", node1, node2, &distance) == 3) {
            addEdge(graph, node1, node2, distance);
        }
    }
    fclose(file);
}

void displayRestaurantInfo(RestaurantInfo *restaurant) {
    printf("\nRestaurant Information:\n");
    printf("+---------------------+--------------------------+\n");
    printf("| Field               | Value                    |\n");
    printf("+---------------------+--------------------------+\n");
    printf("| Type                | %s\n", restaurant->shopType);
    printf("| ID                  | %d\n", restaurant->shopId);
    printf("| Name                | %s\n", restaurant->shopName);
    printf("| Average Score       | %.2lf\n", restaurant->averageScore);
    printf("| Average Price       | %.2lf\n", restaurant->averagePrice);
    printf("| Address             | %s\n", restaurant->address);
    printf("| Phone               | %s\n", restaurant->phone);
    printf("+---------------------+--------------------------+\n");
}

void searchRestaurant(HashTable *hashTable, Graph *graph) {
    char shopName[MAX_SHOP_NAME_LENGTH];
    printf("Enter restaurant name: ");
    fgets(shopName, MAX_SHOP_NAME_LENGTH, stdin);
    shopName[strcspn(shopName, "\n")] = '\0';

    HashNode *foundRestaurant = searchInHashTable(hashTable, shopName);
    if (foundRestaurant) {
        displayRestaurantInfo(&foundRestaurant->restaurant);

        int src = findNodeIndex(graph, "University");
        int dest = findNodeIndex(graph, shopName);
        
        if (src != -1 && dest != -1) {
            double dist[MAX_NODES];
            int prev[MAX_NODES];
            dijkstra(graph, src, dist, prev);
            
            if (dist[dest] != INF) {
                printf("\nShortest Path from University to %s:\n", shopName);
                printPath(graph, prev, dest);
                printf("\nTotal Distance: %.2lf km\n", dist[dest]);
            } else {
                printf("\nNo path found from University to %s\n", shopName);
            }
        }
    } else {
        printf("\nRestaurant not found!\n");
    }
}

void listAllRestaurants(RestaurantInfo restaurants[], int count) {
    printf("\nAll Restaurants (%d):\n", count);
    printf("+-----+------------------+--------+--------+--------+\n");
    printf("| ID  | Name             | Type   | Score  | Price  |\n");
    printf("+-----+------------------+--------+--------+--------+\n");
    
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-16s | %-6s | %-.2lf  | %-.2lf |\n", 
               restaurants[i].shopId,
               restaurants[i].shopName,
               restaurants[i].shopType,
               restaurants[i].averageScore,
               restaurants[i].averagePrice);
    }
    printf("+-----+------------------+--------+--------+--------+\n");
}

void filterByType(RestaurantInfo restaurants[], int count) {
    char type[MAX_SHOP_NAME_LENGTH];
    printf("Enter restaurant type to filter: ");
    fgets(type, MAX_SHOP_NAME_LENGTH, stdin);
    type[strcspn(type, "\n")] = '\0';

    printf("\nRestaurants of type '%s':\n", type);
    printf("+-----+------------------+--------+--------+--------+\n");
    printf("| ID  | Name             | Type   | Score  | Price  |\n");
    printf("+-----+------------------+--------+--------+--------+\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(restaurants[i].shopType, type) == 0) {
            printf("| %-3d | %-16s | %-6s | %-.2lf  | %-.2lf |\n", 
                   restaurants[i].shopId,
                   restaurants[i].shopName,
                   restaurants[i].shopType,
                   restaurants[i].averageScore,
                   restaurants[i].averagePrice);
            found++;
        }
    }
    
    if (!found) {
        printf("|              No restaurants found              |\n");
    }
    printf("+-----+------------------+--------+--------+--------+\n");
}

void recommendRestaurants(RestaurantInfo restaurants[], int count, Graph *graph) {
    char sortBy[20];
    printf("Sort by (score/price/distance): ");
    fgets(sortBy, 20, stdin);
    sortBy[strcspn(sortBy, "\n")] = '\0';

    int src = findNodeIndex(graph, "University");
    if (src == -1) {
        printf("Cannot find source node!\n");
        return;
    }

    int indices[MAX_RESTAURANTS];
    for (int i = 0; i < count; i++) {
        indices[i] = i;
    }

    double dist[MAX_NODES];
    int prev[MAX_NODES];
    dijkstra(graph, src, dist, prev);

    if (strcmp(sortBy, "score") == 0) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (restaurants[indices[i]].averageScore < restaurants[indices[j]].averageScore) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                }
            }
        }
    } else if (strcmp(sortBy, "price") == 0) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (restaurants[indices[i]].averagePrice > restaurants[indices[j]].averagePrice) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                }
            }
        }
    } else if (strcmp(sortBy, "distance") == 0) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                int di = findNodeIndex(graph, restaurants[indices[i]].shopName);
                int dj = findNodeIndex(graph, restaurants[indices[j]].shopName);
                double dist_i = (di != -1) ? dist[di] : INF;
                double dist_j = (dj != -1) ? dist[dj] : INF;
                if (dist_i > dist_j) {
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                }
            }
        }
    } else {
        printf("Invalid sort option!\n");
        return;
    }

    printf("\nTop 5 Recommended Restaurants (sorted by %s):\n", sortBy);
    printf("+-----+------------------+--------+--------+--------+--------+\n");
    printf("| Rank| Name             | Type   | Score  | Price  | Distance|\n");
    printf("+-----+------------------+--------+--------+--------+--------+\n");
    
    int limit = (count < 5) ? count : 5;
    for (int i = 0; i < limit; i++) {
        int idx = indices[i];
        int dest = findNodeIndex(graph, restaurants[idx].shopName);
        double distance = (dest != -1 && dist[dest] != INF) ? dist[dest] : -1;
        
        printf("| %-4d| %-16s | %-6s | %-.2lf  | %-.2lf  | ", 
               i + 1,
               restaurants[idx].shopName,
               restaurants[idx].shopType,
               restaurants[idx].averageScore,
               restaurants[idx].averagePrice);
        
        if (distance >= 0) {
            printf("%.2lf km  |\n", distance);
        } else {
            printf("N/A      |\n");
        }
    }
    printf("+-----+------------------+--------+--------+--------+--------+\n");
}

void displayMenu() {
    printf("\n");
    printf("+==========================================================+\n");
    printf("|         Meituan Restaurant Recommendation System         |\n");
    printf("+==========================================================+\n");
    printf("|  1. Search Restaurant by Name                            |\n");
    printf("|  2. List All Restaurants                                 |\n");
    printf("|  3. Filter Restaurants by Type                           |\n");
    printf("|  4. Recommend Restaurants                                |\n");
    printf("|  5. Exit                                                 |\n");
    printf("+==========================================================+\n");
    printf("Enter your choice (1-5): ");
}

int main() {
    HashTable hashTable = {0};
    Graph graph;
    RestaurantInfo restaurants[MAX_RESTAURANTS];
    int restaurantCount = 0;

    initGraph(&graph);
    
    readShopFile("shop.txt", &hashTable, restaurants, &restaurantCount);
    readDistanceFile("distance.txt", &graph);

    printf("Data loaded successfully!\n");
    printf("Restaurants: %d\n", restaurantCount);
    printf("Graph nodes: %d\n", graph.numNodes);

    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                searchRestaurant(&hashTable, &graph);
                break;
            case 2:
                listAllRestaurants(restaurants, restaurantCount);
                break;
            case 3:
                filterByType(restaurants, restaurantCount);
                break;
            case 4:
                recommendRestaurants(restaurants, restaurantCount, &graph);
                break;
            case 5:
                freeHashTable(&hashTable);
                printf("Exiting... Thank you for using!\n");
                return 0;
            default:
                printf("Invalid choice! Please enter 1-5.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}
