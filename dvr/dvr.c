#include <stdio.h> 
int main() { 
    int cost[20][20], dist[20][20], via[20][20]; 
    int n, i, j, k; 
    printf("Number of nodes: "); 
    scanf("%d", &n); 
    printf("\nEnter cost matrix (Use 999 for infinity):\n"); 
    for (i = 0; i < n; i++) { 
        for (j = 0; j < n; j++) { 
            scanf("%d", &cost[i][j]); 
            cost[i][i] = 0; 
            dist[i][j] = cost[i][j]; // Initialize distance table 
            via[i][j] = j;           // Initialize next hop 
        } 
    } 
    // Distance Vector Algorithm (Bellman-Ford logic) 
    // We check if going from i to j via k is shorter 
    for (k = 0; k < n; k++) { 
        for (i = 0; i < n; i++) { 
            for (j = 0; j < n; j++) { 
                if (dist[i][j] > dist[i][k] + dist[k][j]) { 
                    dist[i][j] = dist[i][k] + dist[k][j]; 
                    via[i][j] = k; 
                } 
            } 
        } 
    } 
    // Displaying the results 
    for (i = 0; i < n; i++) { 
        printf("\nRouting Table for Router %d\n", i + 1); 
        for (j = 0; j < n; j++) { 
            printf("To Node %d via %d : Distance %d\n", j + 1, via[i][j] + 1, dist[i][j]); 
        } 
    } 
    return 0; 
}