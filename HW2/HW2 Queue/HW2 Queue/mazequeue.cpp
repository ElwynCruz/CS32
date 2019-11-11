#include <string>
#include <iostream>
#include <queue>

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
	if (maze[sr][sc] == 'X' || maze[er][ec] == 'X')
		return false;
	queue<Coord> coordQueue;
	Coord a(sr, sc);
	coordQueue.push(a);
	maze[sr][sc] = '*';
	while (!coordQueue.empty())
	{
		Coord current = coordQueue.front();
		coordQueue.pop();
		if (current.r() == er && current.c() == ec)
		{
			return true;
		}
		if (maze[current.r() + 1][current.c()] == '.')
		{
			Coord next(current.r() + 1, current.c());
			coordQueue.push(next);
			maze[current.r() + 1][current.c()] = '*';
		}
		if (maze[current.r()][current.c() - 1] == '.')
		{
			Coord next(current.r(), current.c() - 1);
			coordQueue.push(next);
			maze[current.r()][current.c() - 1] = '*';
		}
		if (maze[current.r() - 1][current.c()] == '.')
		{
			Coord next(current.r() - 1, current.c());
			coordQueue.push(next);
			maze[current.r() - 1][current.c()] = '*';
		}
		if (maze[current.r()][current.c() + 1] == '.')
		{
			Coord next(current.r(), current.c() + 1);
			coordQueue.push(next);
			maze[current.r()][current.c() + 1] = '*';
		}
	}
	return false;

}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
