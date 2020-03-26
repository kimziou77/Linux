#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>
using namespace std;
vector <vector <int>> graph;
vector <bool> visited;
void dfs(int s) {
    visited[s] = true;
    printf("%d ", s);
    for (int i = 0; i < graph[s].size(); i++) {
        int next = graph[s][i];
        if (!visited[next]) {
            dfs(next);
        }
    }
}
void bfs(int s) {
    queue <int> q;
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int num = q.front();
        q.pop();
        printf("%d ", num);
        for (int i = 0; i < graph[num].size(); i++){
            int next = graph[num][i];
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}

int main() {
    int N, M, V;
    scanf("%d %d %d", &N, &M, &V);
    graph.resize(N+1); visited.resize(N+1, false);
    for (int i = 0; i < M; i++) {
        int a, b; scanf("%d %d", &a, &b);
        graph[a].push_back(b);
        graph[b].push_back(a);
    } // graph를 vector로 구현 시, 연결된 정점들을 반드시 정렬해야 함
    for (int i = 1; i <= N; i++) {
        sort(graph[i].begin(), graph[i].end());
    }
    dfs(V);
    printf("\n");
    visited.assign(N+1, false);
    bfs(V);
    return 0;
}
