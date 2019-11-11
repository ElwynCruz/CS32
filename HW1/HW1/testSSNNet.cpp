#include "SSNSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	SSNSet s;
	SSNSet g;
	unsigned long j = 16548;
	s.add(j);
	assert(s.size() == 1);
	s.add(1234567894561234);
	s.add(5432158996);
	s.print();
}
