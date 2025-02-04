#include "iostream"
#include "mazestack.cpp"
#include <cassert>

using namespace std;
// int main() {
//   string maze[10] = {"XXXXXXXXXX", "X........X", "XX.X.XXXXX", "X..X.X...X",
//                      "X..X...X.X", "XXXX.XXX.X", "X.X....XXX", "X..XX.XX.X",
//                      "X...X....X", "XXXXXXXXXX"};

//   if (pathExists(maze, 10, 10, 6, 4, 1, 1))
//     cout << "Solvable!" << endl;
//   else
//     cout << "Out of luck!" << endl;
// }
int main() {
    // Test Case 1: Simple Path Exists
    {
        string maze1[] = {
            "XXXXXXX",
            "X...X.X",
            "XXX.X.X",
            "X.....X",
            "XXXXXXX"
        };
        int nRows1 = 5;
        int nCols1 = 7;
        int sr1 = 1, sc1 = 1;
        int er1 = 3, ec1 = 5;
        bool expected1 = true;
        bool result1 = pathExists(maze1, nRows1, nCols1, sr1, sc1, er1, ec1);
        assert(result1 == expected1);
    }

    // Test Case 2: No Path Exists (Completely Blocked)
    {
        string maze2[] = {
            "XXXXX",
            "X.X.X",
            "XXXXX",
            "X.X.X",
            "XXXXX"
        };
        int nRows2 = 5;
        int nCols2 = 5;
        int sr2 = 1, sc2 = 1;
        int er2 = 3, ec2 = 3;
        bool expected2 = false;
        bool result2 = pathExists(maze2, nRows2, nCols2, sr2, sc2, er2, ec2);
        assert(result2 == expected2);
    }

    // Test Case 3: Start Equals End
    {
        string maze3[] = {
            "XXXXX",
            "X...X",
            "X.X.X",
            "X...X",
            "XXXXX"
        };
        int nRows3 = 5;
        int nCols3 = 5;
        int sr3 = 2, sc3 = 2;
        int er3 = 2, ec3 = 2;
        bool expected3 = true;
        bool result3 = pathExists(maze3, nRows3, nCols3, sr3, sc3, er3, ec3);
        assert(result3 == expected3);
    }

    // Test Case 6: Multiple Paths Available
    {
        string maze6[] = {
            "XXXXXXXX",
            "X......X",
            "X.XXXX.X",
            "X.X..X.X",
            "X.X.XX.X",
            "X......X",
            "XXXXXXXX"
        };
        int nRows6 = 7;
        int nCols6 = 8;
        int sr6 = 1, sc6 = 1;
        int er6 = 5, ec6 = 6;
        bool expected6 = true;
        bool result6 = pathExists(maze6, nRows6, nCols6, sr6, sc6, er6, ec6);
        assert(result6 == expected6);
    }

    // Test Case 7: Maze with Loops
    {
        string maze7[] = {
            "XXXXXXXXX",
            "X...X...X",
            "X.X.X.X.X",
            "X.X...X.X",
            "X.XXXXX.X",
            "X.......X",
            "XXXXXXXXX"
        };
        int nRows7 = 7;
        int nCols7 = 9;
        int sr7 = 1, sc7 = 1;
        int er7 = 5, ec7 = 7;
        bool expected7 = true;
        bool result7 = pathExists(maze7, nRows7, nCols7, sr7, sc7, er7, ec7);
        assert(result7 == expected7);
    }

    // Test Case 9: One Row Maze
    {
        string maze9[] = {
            "..X.X.X.X.X.X.X"
        };
        int nRows9 = 1;
        int nCols9 = 15;
        int sr9 = 0, sc9 = 0;
        int er9 = 0, ec9 = 14;
        bool expected9 = false;
        bool result9 = pathExists(maze9, nRows9, nCols9, sr9, sc9, er9, ec9);
        assert(result9 == expected9);
    }

    // Test Case 10: One Column Maze
    {
        string maze10[] = {
            "X",
            ".",
            "X",
            ".",
            "X",
            ".",
            "X"
        };
        int nRows10 = 7;
        int nCols10 = 1;
        int sr10 = 1, sc10 = 0;
        int er10 = 5, ec10 = 0;
        bool expected10 = false;
        bool result10 = pathExists(maze10, nRows10, nCols10, sr10, sc10,
        er10, ec10); assert(result10 == expected10);
    }

    // Test Case 12: Maze with Multiple Enclosures
    {
        string maze12[] = {
            "XXXXXXXXXX",
            "X..X..X...",
            "X.XX.X.XX.",
            "X..X..X..X",
            "XXXXXX.XXX",
            "X..X..X..X",
            "X.XX.X.XX.",
            "X..X..X...",
            "XXXXXXXXXX"
        };
        int nRows12 = 9;
        int nCols12 = 10;
        int sr12 = 1, sc12 = 1;
        int er12 = 7, ec12 = 9;
        bool expected12 = false;
        bool result12 = pathExists(maze12, nRows12, nCols12, sr12, sc12,
        er12, ec12); assert(result12 == expected12);
    }

    // Test Case 13a: Empty Maze (Zero Rows)
    {
        string* maze13a = nullptr;
        int nRows13a = 0;
        int nCols13a = 0;
        int sr13a = 0, sc13a = 0;
        int er13a = 0, ec13a = 0;
        bool expected13a = false;
        bool result13a = pathExists(maze13a, nRows13a, nCols13a, sr13a,
        sc13a, er13a, ec13a); assert(result13a == expected13a);
    }

    // Test Case 13b: Empty Maze (Zero Columns)
    {
        string maze13b[] = { "." };
        int nRows13b = 1;
        int nCols13b = 1;
        int sr13b = 0, sc13b = 0;
        int er13b = 0, ec13b = 0;
        bool expected13b = true;
        bool result13b = pathExists(maze13b, nRows13b, nCols13b, sr13b,
        sc13b, er13b, ec13b); assert(result13b == expected13b);
    }

    // Test Case 14: Path Requires Backtracking
    {
        string maze14[] = {
            "XXXXXXXXX",
            "X...X...X",
            "X.X...X.X",
            "X.X.X.X.X",
            "X...X...X",
            "XXXXXXXXX"
        };
        int nRows14 = 6;
        int nCols14 = 9;
        int sr14 = 1, sc14 = 1;
        int er14 = 4, ec14 = 7;
        bool expected14 = true;
        bool result14 = pathExists(maze14, nRows14, nCols14, sr14, sc14,
        er14, ec14); assert(result14 == expected14);
    }

    // Test Case 15: Path Exists with Minimal Steps
    {
        string maze15[] = {
            "XXXXX",
            "X...X",
            "XXXXX"
        };
        int nRows15 = 3;
        int nCols15 = 5;
        int sr15 = 1, sc15 = 1;
        int er15 = 1, ec15 = 3;
        bool expected15 = true;
        bool result15 = pathExists(maze15, nRows15, nCols15, sr15, sc15,
        er15, ec15); assert(result15 == expected15);
    }

    // All test cases passed
    cout << "All test cases passed!" << endl;
}
