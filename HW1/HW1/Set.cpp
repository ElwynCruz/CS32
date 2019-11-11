#include "Set.h"
#include <iostream>

Set::Set()
{
	m_size = 0;
	m_maxSize = DEFAULT_MAX_ITEMS;
}	// Create an empty set (i.e., one with no items).

bool Set::empty() const
{
	if (size() == 0)
		return true;
	else 
		return false;
}	// Return true if the set is empty, otherwise false.

int Set::size() const
{
		return m_size;
}		// Return the number of items in the set.

bool Set::insert(const ItemType& value)
{
	if (contains(value) || m_size == m_maxSize)
		return false;
	m_arr[m_size] = value;
	m_size++;
	return true;
}
	// Insert value into the set if it is not already present.  Return
	// true if the value was actually inserted.  Leave the set unchanged
	// and return false if the value was not inserted (perhaps because it
	// was already in the set or because the set has a fixed capacity and
	// is full).

bool Set::erase(const ItemType& value)
{
	if (!contains(value))
		return false;
	int i;
	for (i = 0; i < m_size; i++)	// find place of value in array
	{
		if (m_arr[i] == value)
			break;
	}

	for (;i < m_size - 1; i++)		// shift everything back 1
	{
		m_arr[i] = m_arr[i + 1];
	}
	//m_arr[m_size - 1] = nullptr;		// get rid of last item in array
	m_size--;							// we've removed one item
	return true;
}
	// Remove the value from the set if present.  Return true if the
	// value was removed; otherwise, leave the set unchanged and
	// return false.

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < m_size; i++)
		if (m_arr[i] == value)
			return true;
	return false;
}
	// Return true if the value is in the set, otherwise false.

bool Set::get(int i, ItemType& value) const
{
	if (i < 0 || i >= size())
		return false;

	for (int j = 0; j < m_size; j++)
	{
		int numGreaterThan = 0;
		for (int k = 0; k < m_size; k++)
			if (m_arr[j] > m_arr[k])
				numGreaterThan++;
		if (numGreaterThan == i)
		{
			value = m_arr[j];
			break;
		}
	}
	return true;
}


	// If 0 <= i < size(), copy into value the item in the set that is
	// strictly greater than exactly i items in the set and return true.
	// Otherwise, leave value unchanged and return false.

void Set::swap(Set& other)
{
	ItemType temp;
	int i;
	
	for (i = 0; i < m_size && i < other.m_size; i++)	// while both arrays have something to swap, swap each of them
	{
		temp = other.m_arr[i];
		other.m_arr[i] = m_arr[i];
		m_arr[i] = temp;
	}

	
	int smallSize;
	if (m_size > other.m_size)							// if ours still needs to be swapped over
	{
		smallSize = other.m_size;						// store smaller size
		for (; i < m_size; i++)
			other.insert(m_arr[i]);
		m_size = smallSize;								
	}												
	else                                                  // other still needs to be swapped over
	{
		smallSize = m_size;
		for (; i < other.m_size; i++)
			insert(other.m_arr[i]);
		other.m_size = smallSize;
	}
		
}
		