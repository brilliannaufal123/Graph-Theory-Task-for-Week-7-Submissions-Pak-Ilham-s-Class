```
| Name | NRP | Class |
| Bismantaka Revano Dirgantara | 5025241075 | IUP |
| Naufal Bintang Brillian | 5025241168 | IUP |
| Stephanie Gabriella Adiseputra | 5025241081 | IUP |
|...|...|...|
```

# Graph-Theory-Task-for-Week-7-Submissions-Pak-Ilham-s-Class

## The Knight's Tour

This program implements the Knight’s Tour problem using Warnsdorff’s heuristic and backtracking. The goal is to move a knight across a chessboard so that every square is visited exactly once. 
The program first reads the board size (width, height) and starting position (startX, startY) from input. 
For general cases, the board is represented as a 1D array where all cells are initialized to -1 to mark them as unvisited, and the starting position is marked as 0. 
The algorithm applies Warnsdorff’s heuristic by sorting all possible next moves based on how many onward moves they allow, preferring those with fewer options to avoid dead ends. 
If a move leads to a dead end, backtracking is used to undo the previous move and try another. Once all cells are visited, the program outputs the coordinates in order of visitation. 
The main functions are: 
isSafe(x, y) to check if a move is valid and unvisited
countOnwardMoves(x, y) to count onward moves from a given square
sortMovesByOnwardMoves(x, y, moves, moveCount) to sort moves using Warnsdorff’s heuristic. 

The input format is two lines: the first for board dimensions (width height) and the second for the starting position (startX startY).
Using the classroom's sample input/output, we get the following results:
Example Input 
5 5 
2 2 
Example Output 
```
2 2 
4 1 
2 0 
0 1 
1 3 
3 4 
4 2 
3 0 
1 1 
0 3 
2 4 
4 3 
3 1 
1 0 
0 2 
1 4 
3 3 
2 1 
4 0 
3 2 
4 4 
2 3 
0 4 
1 2 
0 0
```


## Traveling Salesman Problem (TSP)

This program implements the **Traveling Salesman Problem (TSP)** using **Dynamic Programming with Bitmasking**.
The goal is to find the minimum-cost Hamiltonian cycle that visits every node exactly once and returns to the starting point.

The program first reads the number of nodes and edges, followed by each edge’s information (edge ID, start node, end node, and cost).
It then reads the starting node, which is also used as the ending node.
The algorithm constructs an adjacency matrix that stores the minimum cost between every pair of connected nodes and the corresponding edge ID.

To optimize the search, the algorithm uses **Dynamic Programming (DP)** combined with **bitmasking**.
Each state `dp[next][mask]` represents the minimum cost to reach node `next` after visiting all nodes included in `mask`.
The bitmask encodes the set of visited nodes, where each bit represents whether a node has been visited or not.
For each possible subset of visited nodes, the algorithm finds the best transition by trying all possible previous nodes.

The route reconstruction phase traces back through the DP table to recover the sequence of nodes, which is then converted into the sequence of edge IDs corresponding to the actual path.
This approach efficiently reduces the brute-force complexity `O(n!)` into `O(n² * 2ⁿ)`.

Once the optimal route is found, the program outputs the total minimum cost and the ordered list of edge IDs that form the optimal cycle.

---

### The main functions are:

* **buildDistanceMatrix()**
  Creates a cost matrix between all pairs of nodes, keeping only the lowest-cost edge and storing its edge ID.
* **solve()**
  Applies dynamic programming and bitmasking to compute the minimum tour cost.
* **reconstructRoute()**
  Traces back through the DP table to find the sequence of visited nodes and corresponding edge IDs.
* **printResult()**
  Displays the final cost and the sequence of edge IDs that represent the optimal route.

---

### Input Format

The input consists of:

1. The number of nodes `n`
2. The number of edges `e`
3. `e` lines of: `edgeID startNode endNode cost`
4. One line containing the starting node index

> Node numbering can be either 0-based or 1-based.
> The program automatically detects and adjusts the indexing.

---

### Example Input

```
3
4
0 1 2 10
1 2 3 5
2 3 1 7
3 3 1 2
1
```

### Example Output

```
Cost: 17
Route: 0, 1, 3
```

### Explanation

The optimal Hamiltonian cycle starts and ends at node 1:

* Edge 0 → connects node 1 to 2 with cost 10
* Edge 1 → connects node 2 to 3 with cost 5
* Edge 3 → connects node 3 to 1 with cost 2

Total cost = **10 + 5 + 2 = 17**

---
