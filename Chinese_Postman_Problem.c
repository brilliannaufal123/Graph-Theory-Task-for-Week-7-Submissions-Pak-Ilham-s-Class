#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

struct Edge {
    int to;
    int weight;
    int id;
    bool used;
};

class Graph {
    int n;
    vector<vector<Edge>> adj;
    vector<int> edgeName;

public:
    Graph(int nodes) : n(nodes) {
        adj.resize(n);
    }

    void addEdge(int name, int u, int v, int w, int id) {
        edgeName.push_back(name);
        adj[u].push_back({v, w, id, false});
        adj[v].push_back({u, w, id, false});
    }

    pair<vector<string>, int> hierholzer(int start) {
        vector<string> circuit;
        stack<int> st;
        vector<int> path;
        int totalCost = 0;

        st.push(start);

        while (!st.empty()) {
            int u = st.top();
            bool found = false;
            for (auto &edge : adj[u]) {
                if (!edge.used) {
                    edge.used = true;
                    for (auto &rev : adj[edge.to]) {
                        if (rev.id == edge.id && !rev.used) {
                            rev.used = true;
                            break;
                        }
                    }
                    st.push(edge.to);
                    found = true;
                    break;
                }
            }
            if (!found) {
                path.push_back(u);
                st.pop();
            }
        }

        for (int i = path.size() - 1; i > 0; --i) {
            int a = path[i];
            int b = path[i - 1];
            for (auto &edge : adj[a]) {
                if (edge.to == b) {
                    circuit.push_back(edgeName[edge.id]);
                    totalCost += edge.weight;
                    break;
                }
            }
        }

        return {circuit, totalCost};
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    Graph g(n);

    for (int i = 0; i < m; ++i) {
        int name;
        int u, v, w;
        cin >> name >> u >> v >> w;
        g.addEdge(name, u, v, w, i);
    }

    int start;
    cin >> start;

    auto [route, cost] = g.hierholzer(start);

    cout << "Route: ";
    for (auto &e : route) cout << e << " ";
    cout << "\nTotal cost: " << cost << "\n";
}
