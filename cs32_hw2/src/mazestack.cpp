#include <stack>
#include <string>

using namespace std;

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
// The parameters are
// A rectangular maze of Xs and dots that represents the maze. Each string of
// the array is a row of the maze. Each 'X' represents a wall, and each '.'
// represents a walkway. The number of rows in the maze. The number of columns
// in the maze. Each string in the maze parameter must be this length. The
// starting coordinates in the maze: sr, sc; the row number is in the range 0
// through nRows−1, and the column number is in the range 0 through nCols−1. The
// ending coordinates in the maze: er, ec; the row number is in the range 0
// through nRows−1, and the column number is in the range 0 through nCols−1.

class Coord {
public:
  Coord(int r, int c) : m_row(r), m_col(c) {}
  int r() const { return m_row; }
  int c() const { return m_col; }

private:
  int m_row;
  int m_col;
};

static inline void pushIfPossible(string *maze, stack<Coord> &visited,
                                  Coord &coord, int i, int j, int nRows,
                                  int nCols) {
  auto r = coord.r() + i;
  auto c = coord.c() + j;
  auto r_valid = r >= 0 && r < nRows;
  auto c_valid = c >= 0 && c < nCols;
  if (r_valid && c_valid && maze[r][c] == '.') {
    visited.push(Coord(r, c));
  }
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er,
                int ec) {
  if (!maze || !nRows || !nCols) {
    return false;
  }
  stack<Coord> visited({Coord(sr, sc)});
  maze[sr][sc] = 'V';
  while (!visited.empty()) {
    Coord coord = visited.top();
    visited.pop();
    if (coord.r() == er && coord.c() == ec) {
      return true;
    }
    maze[coord.r()][coord.c()] = 'V';
    pushIfPossible(maze, visited, coord, -1, 0, nRows, nCols);
    pushIfPossible(maze, visited, coord, 0, 1, nRows, nCols);
    pushIfPossible(maze, visited, coord, 1, 0, nRows, nCols);
    pushIfPossible(maze, visited, coord, 0, -1, nRows, nCols);
  }
  return false;
}
