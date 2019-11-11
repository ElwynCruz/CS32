

#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set s;
	Set g;
	ItemType t;
	assert(s.empty());
	ItemType x = "9876543";
	ItemType y = "12545";
	assert(!s.get(42, x) && x == "9876543"); // x unchanged by get failure
	s.insert("123456789");
	assert(s.size() == 1);
	assert(s.get(0, x) && x == "123456789");
	ItemType p = "11111";
	g.insert(p);
	s.swap(g);
	assert(s.get(0, x) && x == "11111");
	assert(g.get(0, y) && y == "123456789");
	g.insert(p);
	assert(g.get(1, t) && t == "123456789");
	g.erase("123456789");
	assert(g.size() == 1);
	assert(g.get(0, y) && y == p);
	assert(g.contains(p));
	g.erase(p);
	assert(g.size() == 0 && g.empty());
	cout << "Passed all tests" << endl;
}

