#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

// -----------------------------
// Structure to represent a cell in maze
// -----------------------------
struct Cell {
    int x, y;        // coordinates
    Cell(int _x, int _y) : x(_x), y(_y) {}
};

// -----------------------------
// Maze Class
// -----------------------------
class Maze {
private:
    vector<vector<char>> maze;   // 2D maze grid
    int rows, cols;
    Cell start;   // start point
    Cell end;     // end point

    // Directions for moving (up, down, left, right)
    vector<pair<int,int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};

public:
    // Constructor with maze data
    Maze(vector<vector<char>> grid, Cell s, Cell e) : maze(grid), start(s), end(e) {
        rows = maze.size();
        cols = maze[0].size();
    }

    // Check if cell is valid (inside maze and not a wall '#')
    bool isValidCell(int x, int y, vector<vector<bool>>& visited) {
        return (x >= 0 && x < rows && y >= 0 && y < cols &&
                maze[x][y] != '#' && !visited[x][y]);
    }

    // Print maze (with optional path)
    void printMaze() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << maze[i][j] << " ";
            }
            cout << endl;
        }
    }

    // -----------------------------
    // BFS (Shortest Path)
    // -----------------------------
    bool bfs() {
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        vector<vector<Cell>> parent(rows, vector<Cell>(cols, Cell(-1, -1)));

        queue<Cell> q;
        q.push(start);
        visited[start.x][start.y] = true;

        while (!q.empty()) {
            Cell curr = q.front();
            q.pop();

            // If reached the end
            if (curr.x == end.x && curr.y == end.y) {
                reconstructPath(parent);
                return true;
            }

            // Explore neighbors
            for (auto dir : directions) {
                int nx = curr.x + dir.first;
                int ny = curr.y + dir.second;

                if (isValidCell(nx, ny, visited)) {
                    visited[nx][ny] = true;
                    parent[nx][ny] = curr; // track parent for path
                    q.push(Cell(nx, ny));
                }
            }
        }
        return false; // no path
    }

    // -----------------------------
    // DFS (Recursive)
    // -----------------------------
    bool dfsUtil(Cell curr, vector<vector<bool>>& visited, vector<vector<Cell>>& parent) {
        if (curr.x == end.x && curr.y == end.y)
            return true;

        visited[curr.x][curr.y] = true;

        for (auto dir : directions) {
            int nx = curr.x + dir.first;
            int ny = curr.y + dir.second;

            if (isValidCell(nx, ny, visited)) {
                parent[nx][ny] = curr;
                if (dfsUtil(Cell(nx, ny), visited, parent)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool dfs() {
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        vector<vector<Cell>> parent(rows, vector<Cell>(cols, Cell(-1, -1)));

        if (dfsUtil(start, visited, parent)) {
            reconstructPath(parent);
            return true;
        }
        return false;
    }

    // -----------------------------
    // Reconstruct path (mark '*' in maze)
    // -----------------------------
    void reconstructPath(vector<vector<Cell>>& parent) {
        Cell curr = end;
        while (!(curr.x == start.x && curr.y == start.y)) {
            maze[curr.x][curr.y] = '*';
            curr = parent[curr.x][curr.y];
        }
        maze[start.x][start.y] = 'S';
        maze[end.x][end.y] = 'E';
    }
};

// -----------------------------
// Main Function
// -----------------------------
int main() {
    // Example Maze
    // S = Start, E = End, . = Path, # = Wall
    vector<vector<char>> grid = {
        {'S', '.', '.', '#', '.', '.'},
        {'.', '#', '.', '#', '.', '#'},
        {'.', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '#', '#', '.'},
        {'#', '.', '.', '.', '#', 'E'}
    };

    Cell start(0,0);
    Cell end(4,5);

    Maze mz(grid, start, end);

    cout << "Original Maze:\n";
    mz.printMaze();

    cout << "\nSolving with BFS (Shortest Path)...\n";
    if (mz.bfs()) {
        mz.printMaze();
    } else {
        cout << "No path found with BFS!\n";
    }

    cout << "\nSolving with DFS...\n";
    Maze mz2(grid, start, end); // fresh maze for DFS
    if (mz2.dfs()) {
        mz2.printMaze();
    } else {
        cout << "No path found with DFS!\n";
    }

    return 0;
}
