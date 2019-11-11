#include "Set.h"

Set::Set() 
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Set::Set(const Set &other)
{
	SetNode *p = other.head;
	while (p != nullptr)
	{
		insert(p->value);				// insert each node into our set
		p = p->next;					
	}
}

Set::~Set()
{
	while (m_size > 0)
	{
		erase(head->value);				// erase will decrement m_size
	}
}

Set& Set::operator=(const Set &other) 
{
	while (m_size > 0)
	{
		erase(head->value);				// get rid of all nodes in our list
	}
	SetNode *p = other.head;
	while (p != nullptr)
	{
		insert(p->value);				// insert each node from other list into ours
		p = p->next;
	}
	return *this;
}

bool Set::empty() const
{
	if (head == nullptr)
	{
		return true;
	}
	return false;
}
int Set::size() const
{
	return m_size;
}
bool Set::insert(const ItemType& value)
{
	if (contains(value))
		return false;
	if (m_size == 0)							// no other items in our set
	{
		SetNode *v = new SetNode;
		v->value = value;
		v->next = nullptr;
		v->prev = nullptr;
		head = v;
		tail = v;
	}
	else                                       // insert at end of linked list
	{
		SetNode *v = new SetNode;
		v->value = value;
		v->next = nullptr;
		v->prev = tail;
		tail->next = v;
		tail = v;
	}
	m_size++;
	return true;

}
bool Set::erase(const ItemType& value)
{
	if (!contains(value))
		return false;

	SetNode *p = head;
	while (p != nullptr)			// loop through to find node we need to delete
	{
		if (p->value == value)
			break;
		p = p->next;
	}
	
	if (p != head)              // if we're not removing the first item
	{
		p->prev->next = p->next;		// change the node before node we're going to delete's next to point to the node after the one we're deleting
	}
	if (p != tail)				// if we're not removing the last item
	{
		p->next->prev = p->prev;			// change node after node we're going to delete's prev to point to node before the one we're deleting
	}
	if (p == head)		// removing the first item
	{
		head = p->next;
	}
	if (p == tail)		// removing the last item						seperate ifs in case if there is one item in list, because then we need to change head and tail
	{
		tail = p->prev;
	}
	delete p;
	m_size--;
	return true;
}
bool Set::contains(const ItemType& value) const
{
	SetNode *p = head;
	while (p != nullptr)					
	{
		if (p->value == value)						//found the node
			return true;
		else                                        // keep going
			p = p->next;
	}
	return false;
}
bool Set::get(int pos, ItemType& value) const
{
	if (pos >= m_size)
		return false;

	SetNode *p = head;
	while (p != nullptr)			
	{
		SetNode *q = head;
		int numGreaterThan = 0;
		while (q != nullptr)											// we're going to compare p's value to every other value until we find one greater than exactly pos values
		{
			if (p->value > q->value)							
				numGreaterThan++;
			q = q->next;
		}
		if (numGreaterThan == pos)										// if we find exactly pos values we are greater than, we found our item
		{
			value = p->value;
			break;
		}
		p = p->next;
	}
	return true;
}
void Set::swap(Set& other)
{
	SetNode *tempHead = head;							// swap the heads and tails, so it points to the other linked list
	SetNode *tempTail = tail;
	int tempSize = m_size;
	head = other.head;
	tail = other.tail;
	other.head = tempHead;
	other.tail = tempTail;
	m_size = other.m_size;								//swap sizes
	other.m_size = tempSize;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set united;
	for (int i = 0; i < s1.size(); i++)				// insert all of s1 into united
	{
		ItemType x; 
		s1.get(i, x);
		united.insert(x);
	}
	for (int i = 0; i < s2.size(); i++)				// insert s2 into united (any repeats won't be inserted since our insert checks for repeats)
	{
		ItemType x;
		s2.get(i, x);
		united.insert(x);
	}
	result = united;								// use our assignment operator to get rid of anything previously in result and fill it with our values from s1 and s2
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set subtracted = s1;
	for (int i = 0; i < s2.size(); i++)
	{
		ItemType x;
		s1.get(i, x);
		subtracted.erase(x);						// erase every value in s2 from s1
													// erasing any value in s2 but not s1 just returns false
	}
	result = subtracted;
}