#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (ll)4e18;

class TSP {
private:
    int n, e;                   // number of nodes and edges
    int start;                  // starting node index
    vector<vector<ll>> dist;    // minimum cost matrix between nodes
    vector<vector<int>> edgeId; // edge id corresponding to minimum cost
    vector<vector<ll>> dp;      // dp[next][mask]
    vector<vector<int>> parent; // to reconstruct the path
    ll minTourCost = INF;
    vector<int> routeIds;       // edge ids in final route

    // Helper: build minimum adjacency matrix from raw edges
    void buildDistanceMatrix(const vector<tuple<int,int,int,ll>>& edges, bool oneBased) {
        dist.assign(n, vector<ll>(n, INF));
        edgeId.assign(n, vector<int>(n, -1));

        for (int i = 0; i < n; ++i) dist[i][i] = 0;

        for (auto [id, u, v, w] : edges) {
            if (oneBased) { u--; v--; }
            if (w < dist[u][v]) {
                dist[u][v] = w;
                edgeId[u][v] = id;
            }
            if (w < dist[v][u]) {
                dist[v][u] = w;
                edgeId[v][u] = id;
            }
        }
    }

public:
    // Constructor: read input and build distance matrix
    TSP() {
        cin >> n >> e;
        vector<tuple<int,int,int,ll>> edges;
        edges.reserve(e);

        int minLabel = INT_MAX, maxLabel = INT_MIN;
        for (int i = 0; i < e; ++i) {
            int id, u, v; ll w;
            cin >> id >> u >> v >> w;
            edges.emplace_back(id, u, v, w);
            minLabel = min({minLabel, u, v});
            maxLabel = max({maxLabel, u, v});
        }
        cin >> start;

        // auto-detect 1-based or 0-based indexing
        bool oneBased = (minLabel == 1 && maxLabel == n);
        if (oneBased) start--;

        buildDistanceMatrix(edges, oneBased);
    }

    // Solve TSP using DP + Bitmask
    void solve() {
        int FULL = (1 << n);
        dp.assign(n, vector<ll>(FULL, INF));
        parent.assign(n, vector<int>(FULL, -1));

        // Base case: direct path from start to other nodes
        for (int end = 0; end < n; ++end) {
            if (end == start) continue;
            int mask = (1 << start) | (1 << end);
            if (dist[start][end] < INF) {
                dp[end][mask] = dist[start][end];
            }
        }

        // Build subsets of increasing size
        for (int mask = 0; mask < FULL; ++mask) {
            if (!(mask & (1 << start))) continue; // must contain start
            for (int last = 0; last < n; ++last) {
                if (last == start || !(mask & (1 << last))) continue;
                int prevMask = mask ^ (1 << last);
                for (int k = 0; k < n; ++k) {
                    if (k == start || k == last || !(mask & (1 << k))) continue;
                    ll newCost = dp[k][prevMask] + dist[k][last];
                    if (newCost < dp[last][mask]) {
                        dp[last][mask] = newCost;
                        parent[last][mask] = k;
                    }
                }
            }
        }

        // Close the cycle back to start
        int END_STATE = FULL - 1;
        int lastNode = -1;
        for (int i = 0; i < n; ++i) {
            if (i == start) continue;
            ll tourCost = dp[i][END_STATE] + dist[i][start];
            if (tourCost < minTourCost) {
                minTourCost = tourCost;
                lastNode = i;
            }
        }

        reconstructRoute(lastNode, END_STATE);
    }

    // Reconstruct path using parent table
    void reconstructRoute(int lastNode, int mask) {
        vector<int> path;
        path.push_back(start);
        int curr = lastNode;

        // Backtrack through parent table
        while (curr != -1) {
            path.push_back(curr);
            int next = parent[curr][mask];
            mask ^= (1 << curr);
            curr = next;
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        // Convert node sequence to edge IDs
        for (int i = 0; i + 1 < (int)path.size(); ++i) {
            int u = path[i], v = path[i + 1];
            routeIds.push_back(edgeId[u][v]);
        }
    }

    // Print result
    void printResult() {
        if (minTourCost >= INF) {
            cout << "Cost: -1\nRoute:\n";
            return;
        }
        cout << "Cost: " << minTourCost << "\n";
        cout << "Route: ";
        for (size_t i = 0; i < routeIds.size(); ++i) {
            if (i) cout << ", ";
            cout << routeIds[i];
        }
        cout << "\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    Just a test case to prove the code works:
    Here we used from the Classroom

    3
4
0 1 2 10
1 2 3 5
2 3 1 7
3 3 1 2
1

    */

    TSP tsp;
    tsp.solve();
    tsp.printResult();

    return 0;
}
