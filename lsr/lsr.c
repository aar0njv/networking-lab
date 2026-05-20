#include <stdio.h>

int main() {

    int cost[20][20], dist[20], visited[20];
    int n, i, j, count, mindist, nextnode;
    int start;

    printf("Number of nodes: ");
    scanf("%d", &n);

    printf("\nEnter cost matrix (Use 999 for infinity):\n");

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }

    printf("\nEnter source router: ");
    scanf("%d", &start);

    // Initialize distance and visited arrays
    for(i = 0; i < n; i++) {
        dist[i] = cost[start][i];
        visited[i] = 0;
    }

    visited[start] = 1;

    count = 1;

    // Dijkstra Algorithm
    while(count < n - 1) {

        mindist = 999;

        // Find nearest unvisited node
        for(i = 0; i < n; i++) {
            if(dist[i] < mindist && !visited[i]) {
                mindist = dist[i];
                nextnode = i;
            }
        }

        // Mark visited
        visited[nextnode] = 1;

        for(i = 0; i < n; i++) {
            if(!visited[i]) {
                if(mindist + cost[nextnode][i] < dist[i]) {
                    dist[i] = mindist + cost[nextnode][i];
                }
            }
        }

        count++;
    }

    // Display shortest paths
    printf("\nShortest Paths from Router %d:\n", start);
    for(i = 0; i < n; i++) {
        printf("To Node %d : Distance %d\n", i, dist[i]);
    }

    return 0;
}