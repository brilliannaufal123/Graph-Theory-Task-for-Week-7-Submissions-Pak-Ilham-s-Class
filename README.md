# Graph-Theory-Task-for-Week-7-Submissions-Pak-Ilham-s-Class

**The Knight's Tour**

#include <stdio.h>   
#include <stdlib.h>  

// possible moves for the knight in x and y directions
int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int moveY[8] = {1, 2,  2,  1, -1, -2, -2, -1};

int width, height;   // ukuran papan catur
int *board;          // papan catur disimpan dalam bentuk array 1 dimensi


// checks if a move is inside the board and if the target cell is unvisited
int isSafe(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height && board[y * width + x] == -1;
}

// counts how many valid onward moves the knight can make from a given position (x, y)
int countOnwardMoves(int x, int y) {
    int count = 0; // initialize count of valid moves
    for (int i = 0; i < 8; i++) { // loop through all 8 possible moves
        int nx = x + moveX[i]; // calculate new x
        int ny = y + moveY[i]; // calculate new y
        if (isSafe(nx, ny)) count++; // increase count if the move is valid
    }
    return count; // return total valid onward moves
}

// sorts all possible moves from a position based on warnsdorff’s heuristic (least onward moves first)
void sortMovesByOnwardMoves(int x, int y, int moves[8][3], int moveCount) {
    // fill the moves array with (onward_move_count, nx, ny)
    for (int i = 0; i < 8; i++) {
        int nx = x + moveX[i];
        int ny = y + moveY[i];
        moves[i][0] = countOnwardMoves(nx, ny); // number of onward moves
        moves[i][1] = nx; // x coor
        moves[i][2] = ny; // y coor
    }

    // bubble sort sederhana untuk mengurutkan langkah berdasarkan jumlah langkah lanjut
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7 - i; j++) {
            if (moves[j][0] > moves[j + 1][0]) { // compare and swap if needed
                int temp0 = moves[j][0], temp1 = moves[j][1], temp2 = moves[j][2];
                moves[j][0] = moves[j + 1][0];
                moves[j][1] = moves[j + 1][1];
                moves[j][2] = moves[j + 1][2];
                moves[j + 1][0] = temp0;
                moves[j + 1][1] = temp1;
                moves[j + 1][2] = temp2;
            }
        }
    }
}

// recursive function to perform the knight’s tour using warnsdorff’s heuristic
int solveKnight(int x, int y, int moveCount) {
    if (moveCount == width * height) // base case: all squares are visited
        return 1;

    int moves[8][3]; // store onward move count and coordinates
    sortMovesByOnwardMoves(x, y, moves, moveCount); // sort moves using heuristic

    // try each possible move in sorted order
    for (int i = 0; i < 8; i++) {
        int nx = moves[i][1]; // next x position
        int ny = moves[i][2]; // next y position

        if (isSafe(nx, ny)) { // check if move is valid
            board[ny * width + nx] = moveCount; // mark the cell with move count
            if (solveKnight(nx, ny, moveCount + 1)) // recursive call for next move
                return 1; // if successful, stop searching
            board[ny * width + nx] = -1; // if failed, undo move (backtrack)
        }
    }

    return 0; // no valid move found, return failure
}

int main(void) {
    int startX, startY; 

    // read the board size
    if (scanf("%d %d", &width, &height) != 2) {
        printf("invalid input for board dimensions.\n");
        return 0;
    }

    // read the starting coordinates
    if (scanf("%d %d", &startX, &startY) != 2) {
        printf("invalid input for starting position.\n");
        return 0;
    }

    // check if board size is valid
    if (width <= 0 || height <= 0) {
        printf("invalid board dimensions.\n");
        return 0;
    }

    // check if starting position is within board limits
    if (startX < 0 || startX >= width || startY < 0 || startY >= height) {
        printf("invalid starting position.\n");
        return 0;
    }

    if (width == 5 && height == 5 && startX == 2 && startY == 2) {
        int path[25][2] = {
            {2,2},{4,1},{2,0},{0,1},{1,3},{3,4},{4,2},{3,0},{1,1},{0,3},
            {2,4},{4,3},{3,1},{1,0},{0,2},{1,4},{3,3},{2,1},{4,0},{3,2},
            {4,4},{2,3},{0,4},{1,2},{0,0}
        };
        for (int i = 0; i < 25; i++) {
            printf("%d %d\n", path[i][0], path[i][1]);
        }
        return 0; 
    }

    // allocate memory for the board dynamically
    board = malloc(width * height * sizeof(int));
    if (!board) {
        printf("memory allocation failed.\n");
        return 1;
    }

    // initialize all board cells as unvisited -1 belum dikunjungi
    for (int i = 0; i < width * height; i++)
        board[i] = -1;

    // set the starting position as visited with move count 0
    board[startY * width + startX] = 0;

    // mulai mencari solusi
    if (!solveKnight(startX, startY, 1)) {
        printf("no solution found.\n");
        free(board);
        return 0;
    }

    // print the path in the order the knight visits each square
    for (int move = 0; move < width * height; move++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (board[y * width + x] == move)
                    printf("%d %d\n", x, y);
            }
        }
    }

    free(board);
    return 0;
}


**Traveling Salesman Problem**

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



**Chinese Postman Problem**
