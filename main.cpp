#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Edge {
    int to;
    int rev;
    int cap;
    ll cost;
};

class MinCostMaxFlow {
public:
    explicit MinCostMaxFlow(int n) : n_(n), g_(n) {}

    void addEdge(int u, int v, int cap, ll cost) {
        Edge a{v, (int)g_[v].size(), cap, cost};
        Edge b{u, (int)g_[u].size(), 0, -cost};
        g_[u].push_back(a);
        g_[v].push_back(b);
    }

    pair<int, ll> minCostMaxFlow(int s, int t) {
        const ll INF = (ll)4e18;
        int flow = 0;
        ll cost = 0;

        vector<ll> dist(n_);
        vector<int> inq(n_), pvNode(n_), pvEdge(n_);

        while (true) {
            fill(dist.begin(), dist.end(), INF);
            fill(inq.begin(), inq.end(), 0);
            queue<int> q;
            dist[s] = 0;
            q.push(s);
            inq[s] = 1;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = 0;

                for (int i = 0; i < (int)g_[u].size(); ++i) {
                    const Edge &e = g_[u][i];
                    if (e.cap <= 0) continue;
                    if (dist[u] != INF && dist[e.to] > dist[u] + e.cost) {
                        dist[e.to] = dist[u] + e.cost;
                        pvNode[e.to] = u;
                        pvEdge[e.to] = i;
                        if (!inq[e.to]) {
                            inq[e.to] = 1;
                            q.push(e.to);
                        }
                    }
                }
            }

            if (dist[t] == INF) break;

            int add = INT_MAX;
            for (int v = t; v != s; v = pvNode[v]) {
                const Edge &e = g_[pvNode[v]][pvEdge[v]];
                add = min(add, e.cap);
            }

            for (int v = t; v != s; v = pvNode[v]) {
                Edge &e = g_[pvNode[v]][pvEdge[v]];
                Edge &re = g_[e.to][e.rev];
                e.cap -= add;
                re.cap += add;
                cost += 1LL * add * e.cost;
            }
            flow += add;
        }

        return {flow, cost};
    }

private:
    int n_;
    vector<vector<Edge>> g_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N;
    cin >> M >> N;

    vector<vector<int>> T(N + 1, vector<int>(M + 1));
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            cin >> T[i][j];
        }
    }

    // 节点编号：
    // 1..N: 每辆车
    // N+1 .. N+M*N: (技师j的第k个服务位置)
    // S, T 为源汇
    int base = N;
    int slots = M * N;
    int S = base + slots + 1;
    int TT = S + 1;
    int V = TT + 1;

    MinCostMaxFlow mcmf(V);

    for (int i = 1; i <= N; ++i) {
        mcmf.addEdge(S, i, 1, 0);
    }

    for (int j = 1; j <= M; ++j) {
        for (int k = 1; k <= N; ++k) {
            int slotNode = base + (j - 1) * N + k;
            mcmf.addEdge(slotNode, TT, 1, 0);
            for (int i = 1; i <= N; ++i) {
                mcmf.addEdge(i, slotNode, 1, 1LL * T[i][j] * k);
            }
        }
    }

    auto [flow, minCost] = mcmf.minCostMaxFlow(S, TT);
    (void)flow; // 理论上应为 N

    cout.setf(std::ios::fixed);
    cout << setprecision(2) << (double)minCost / (double)N << '\n';
    return 0;
}
