#include <string>
#include <iostream>
using namespace std;


class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{

	if (sr == er && sc == ec)
	{
		return true;
	}

	if (maze[sr][sc] != '.')
		return false;

	Coord NORTH(sr-1, sc);
	Coord EAST(sr, sc + 1);
	Coord SOUTH(sr + 1, sc);
	Coord WEST(sr, sc - 1);

	Coord DIRECTIONS[4] = { SOUTH, WEST, NORTH, EAST };

	maze[sr][sc] = '*';
	for (int i = 0; i < 4; i++)
	{
		Coord curDirection = DIRECTIONS[i];
		if (maze[curDirection.r()][curDirection.c()] == '.')
			if (pathExists(maze, nRows, nCols, curDirection.r(), curDirection.c(), er, ec))
				return true;
	}
	return false;
}	
