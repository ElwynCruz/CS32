#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set ss;  // ItemType is std::string
	ss.insert("ccc");
	ss.insert("aaa");
	ss.insert("bbb");
	ItemType x = "xxx";
	assert(!ss.get(3, x) && x == "xxx");  // x is unchanged
	assert(ss.get(1, x) && x == "bbb");   // "bbb" is greater than
											// exactly 1 item
	assert(ss.size() == 3);
	assert(ss.contains("ccc") == true);
	ss.erase("ccc");
	assert(ss.get(0, x) && x == "aaa");
	assert(ss.get(1, x) && x == "bbb");
	Set cc;
	cc.insert("lol");
	ss.swap(cc);
	assert(ss.size() == 1);
	assert(ss.get(0, x) && x == "lol");
	ss.erase("lol");
	assert(ss.empty());
	ss.insert("aaa");
	ss.insert("bbb");
	subtract(ss, cc, ss);
	assert(ss.empty());
	ss.insert("ddd");
	ss.insert("ccc");
	unite(ss, cc, ss);
	assert(ss.get(3, x) && x == "ddd");
	assert(!ss.erase("roti"));
	cc = ss;
	assert(cc.get(3, x) && x == "ddd");
	Set ll = ss;
	assert(ll.get(1, x) && x == "bbb");
	cout << "All tests passed\n";
}