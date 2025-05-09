// VinDelivery
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#define INF 1e9

typedef struct {
    int   V;
    double **mat;
} Graph;

// append to graph_traversal.txt
void logTraversal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    FILE *fp = fopen("graph_traversal.txt", "a");
    if (!fp) { perror("log open"); va_end(args); return; }
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    fclose(fp);
    va_end(args);
}

// Read adjacency-matrix from file
Graph* createGraph(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) { perror("open input"); exit(1); }
    Graph *g = malloc(sizeof *g);
    if (fscanf(f, "%d", &g->V)!=1) {
        fprintf(stderr,"bad format\n"); exit(1);
    }
    int V = g->V;
    g->mat = malloc(V * sizeof(double*));
    for (int i = 0; i < V; i++) {
        g->mat[i] = malloc(V * sizeof(double));
        for (int j = 0; j < V; j++) {
            double w;
            if (fscanf(f, "%lf", &w)!=1) {
                fprintf(stderr,"bad matrix\n"); exit(1);
            }
            g->mat[i][j] = (w > 0 ? w : 0.0);
        }
    }
    fclose(f);
    return g;
}

// free
void freeGraph(Graph *g) {
    for (int i = 0; i < g->V; i++)
        free(g->mat[i]);
    free(g->mat);
    free(g);
}

// Dijkstra
void dijkstra(Graph *g, int src, int dest) {
    int V = g->V;
    double *dist   = malloc(V*sizeof(double));
    bool   *used   = malloc(V*sizeof(bool));
    int    *parent = malloc(V*sizeof(int));
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        used[i] = false;
        parent[i] = -1;
    }
    dist[src] = 0;
    for (int it = 0; it < V; it++) {
        // pick unvisited node with min dist
        double best = INF;
        int u = -1;
        for (int i = 0; i < V; i++)
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        if (u<0) break;
        used[u] = true;
        if (u == dest) break;
        // relax edges
        for (int v = 0; v < V; v++) {
            double w = g->mat[u][v];
            if (w > 0 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }
    if (dist[dest] >= INF) {
        printf("No path from %d to %d\n", src, dest);
        logTraversal("Dijkstra %d->%d | no path", src, dest);
    } else {
        printf("Distance = %.2f\n", dist[dest]);
        int *path = malloc(V*sizeof(int));
        int  L = 0;
        for (int at = dest; at != -1; at = parent[at])
            path[L++] = at;
        printf("Path:");
        // reverse
        for (int i = L-1; i >= 0; i--)
            printf(" %d", path[i]);
        printf("\n");
        // build log line
        char buf[1024];
        int pos = snprintf(buf, sizeof(buf),
                           "Dijkstra %d->%d | dist=%.2f | path:",
                           src, dest, dist[dest]);
        for (int i = L-1; i >= 0; i--)
            pos += snprintf(buf+pos, sizeof(buf)-pos, " %d", path[i]);
        logTraversal("%s", buf);
        free(path);
    }
    free(dist); free(used); free(parent);
}

// BFS + log
void bfs(Graph *g, int start) {
    int V = g->V;
    bool *vis   = calloc(V, sizeof(bool));
    int  *q     = malloc(V*sizeof(int));
    int   head=0, tail=0;
    int  *order = malloc(V*sizeof(int)), L=0;

    vis[start] = true;
    q[tail++] = start;
    while (head < tail) {
        int u = q[head++];
        order[L++] = u;
        for (int v = 0; v < V; v++) {
            if (g->mat[u][v] > 0 && !vis[v]) {
                vis[v] = true;
                q[tail++] = v;
            }
        }
    }
    printf("BFS order:");
    for (int i=0;i<L;i++) printf(" %d", order[i]);
    printf("\n");
    // log
    char buf[1024];
    int pos = snprintf(buf, sizeof(buf), "BFS from %d:", start);
    for (int i=0;i<L;i++)
        pos += snprintf(buf+pos, sizeof(buf)-pos, " %d", order[i]);
    logTraversal("%s", buf);

    free(vis); free(q); free(order);
}

//  DFS util
void dfsUtil(Graph *g, int u, bool *vis, int *order, int *Lp) {
    vis[u] = true;
    order[(*Lp)++] = u;
    for (int v = 0; v < g->V; v++) {
        if (g->mat[u][v] > 0 && !vis[v])
            dfsUtil(g, v, vis, order, Lp);
    }
}
//     DFS + log
void dfs(Graph *g, int start) {
    int V = g->V;
    bool *vis   = calloc(V,sizeof(bool));
    int  *order = malloc(V*sizeof(int)), L=0;

    dfsUtil(g, start, vis, order, &L);
    printf("DFS order:");
    for (int i=0;i<L;i++) printf(" %d", order[i]);
    printf("\n");
    // log
    char buf[1024];
    int pos = snprintf(buf, sizeof(buf), "DFS from %d:", start);
    for (int i=0;i<L;i++)
        pos += snprintf(buf+pos, sizeof(buf)-pos, " %d", order[i]);
    logTraversal("%s", buf);

    free(vis); free(order);
}

// addCity
void addCity(Graph *g) {
    int oldV = g->V, newV = oldV+1;
    g->mat = realloc(g->mat, newV*sizeof(double*));
    for (int i=0;i<oldV;i++){
        g->mat[i] = realloc(g->mat[i], newV*sizeof(double));
        g->mat[i][newV-1] = 0.0;
    }
    g->mat[newV-1] = malloc(newV*sizeof(double));
    for (int j=0;j<newV;j++) g->mat[newV-1][j]=0.0;
    g->V = newV;
    printf("Added city %d\n", newV-1);
    logTraversal("Added city %d", newV-1);
}

// addRoad
void addRoad(Graph *g, int u, int v, double w) {
    if (u<0||u>=g->V||v<0||v>=g->V){
        printf("Invalid nodes\n");
        return;
    }
    g->mat[u][v] = w;
    printf("Added road %d->%d w=%.2f\n", u, v, w);
    logTraversal("Added road %d->%d w=%.2f", u, v, w);
}

// 5. cycle detection
bool cycleDFS(Graph *g, int u, int *state) {
    state[u]=1;  // visiting
    for (int v=0; v<g->V; v++){
        if (g->mat[u][v]>0) {
            if (state[v]==1) return true;
            if (state[v]==0 && cycleDFS(g,v,state))
                return true;
        }
    }
    state[u]=2;  // done
    return false;
}

void hasCycle(Graph *g) {
    int V = g->V;
    int *state = calloc(V, sizeof(int));
    bool found = false;
    for (int i=0;i<V;i++){
        if (state[i]==0 && cycleDFS(g,i,state)) {
            found = true;
            break;
        }
    }
    printf(found? "Cycle detected\n":"No cycle\n");
    logTraversal(found? "Cycle detected":"No cycle");
    free(state);
}

int main() {
    Graph *g = createGraph("city_graph.txt");
    while (1) {
        printf(
            "\nMenu:\n"
            "1) Dijkstra\n"
            "2) BFS\n"
            "3) DFS\n"
            "4) Add city\n"
            "5) Add road\n"
            "6) Detect cycle\n"
            "7) Quit\n"
            "Choose> ");
        int cmd;
        if (scanf("%d", &cmd)!=1) break;
        if (cmd==7) break;
        switch(cmd) {
            case 1: {
                int s,t;
                printf("src dest> ");
                scanf("%d %d",&s,&t);
                if (s<0||s>=g->V||t<0||t>=g->V)
                    printf("Invalid nodes\n");
                else
                    dijkstra(g,s,t);
                break;
            }
            case 2: {
                int s;
                printf("start> ");
                scanf("%d",&s);
                if (s<0||s>=g->V) printf("Invalid node\n");
                else bfs(g,s);
                break;
            }
            case 3: {
                int s;
                printf("start> ");
                scanf("%d",&s);
                if (s<0||s>=g->V) printf("Invalid node\n");
                else dfs(g,s);
                break;
            }
            case 4:
                addCity(g);
                break;
            case 5: {
                int u,v; double w;
                printf("u v w> ");
                scanf("%d %d %lf",&u,&v,&w);
                addRoad(g,u,v,w);
                break;
            }
            case 6:
                hasCycle(g);
                break;
            default:
                printf("Invalid option\n");
        }
    }
    freeGraph(g);
    printf("Bye!\n");
    return 0;
}
