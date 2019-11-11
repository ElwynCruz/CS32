#include <string>

using ItemType = std::string;

class Set
{
public:
	Set();
	Set(const Set &other);
	~Set();
	Set& operator=(const Set &other);
	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);
private:
	struct SetNode
	{
		SetNode *next, *prev;			// pointers to previous and next items in list
		ItemType value;
	};
	SetNode *head, *tail;				// pointer to beginning and end of our list
	int m_size;
};


void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);