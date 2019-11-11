#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			m_grid[r][c] = '.';
}

bool History::record(int r, int c)			// when a zombie dies, run this function to store it's r,c in m_grid
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
		return false;
	char& gridChar = m_grid[r-1][c-1];
	switch (gridChar)
	{
		case '.':  gridChar = 'A'; break;
		case 'Z':  break;				// If 'Z' leave as 'Z'
		default:   gridChar++; break;  // 'B' through 'Z'
	}

	return true;

}

void History::display() const
{
	int r, c;
	clearScreen();
	for (r = 0; r < m_rows; r++)
	{
		for (c = 0; c < m_cols; c++)
			cout << m_grid[r][c];
		cout << endl;
	}
	cout << endl;
	
}
