#include <stdio.h>
#include <limits.h>
#define N 10
#define INF 999

void printPath(int parent[], int j) {
    if (parent[j] == -1) { printf("%d", j + 1); return; }
    printPath(parent, parent[j]);
    printf(" -> %d", j + 1);
}

// ----- Distance Vector -----
void distanceVector(int c[N][N], int n, int s, int d) {
    int dist[N][N], next[N][N], i, j, k;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            dist[i][j] = c[i][j];
            next[i][j] = (c[i][j] != INF && i != j) ? j : -1;
        }

    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }

    printf("\n[Distance Vector]\nShortest distance from %d to %d: %d\nPath: ", s + 1, d + 1, dist[s][d]);
    int cur = s;
    while (cur != d && cur != -1) {
        printf("%d -> ", cur + 1);
        cur = next[cur][d];
    }
    printf("%d\n", d + 1);
}

// ----- Link State (Dijkstra) -----
void dijkstra(int c[N][N], int n, int s, int d) {
    int dist[N], visited[N] = {0}, parent[N], i, j, min, u;
    for (i = 0; i < n; i++) {
        dist[i] = c[s][i];
        parent[i] = (c[s][i] != INF && i != s) ? s : -1;
    }
    visited[s] = 1;

    for (i = 0; i < n - 1; i++) {
        min = INF;
        for (j = 0; j < n; j++)
            if (!visited[j] && dist[j] < min) { min = dist[j]; u = j; }
        visited[u] = 1;
        for (j = 0; j < n; j++)
            if (!visited[j] && dist[u] + c[u][j] < dist[j]) {
                dist[j] = dist[u] + c[u][j];
                parent[j] = u;
            }
    }

    printf("\n[Link State]\nShortest distance from %d to %d: %d\nPath: ", s + 1, d + 1, dist[d]);
    printPath(parent, d);
    printf("\n");
}

int main() {
    int n, c[N][N], s, d;
    printf("Enter number of routers: ");
    scanf("%d", &n);
    printf("Enter cost matrix (999 for no link):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &c[i][j]);
    printf("Enter source router (1-%d): ", n);
    scanf("%d", &s);
    printf("Enter destination router (1-%d): ", n);
    scanf("%d", &d);
    s--; d--;
    distanceVector(c, n, s, d);
    dijkstra(c, n, s, d);
    return 0;
}
