#include "SSNSet.h"
#include <iostream>

SSNSet::SSNSet()
{

}// Create an empty SSN set.

bool SSNSet::add(unsigned long ssn)
{
	return m_Set.insert(ssn);
}
	// Add an SSN to the SSNSet.  Return true if and only if the SSN
	// was actually added.

int SSNSet::size() const 
{
	return m_Set.size();
}  // Return the number of SSNs in the SSNSet.

void SSNSet::print() const
{
	for (int i = 0; i < size(); i++)
	{
		unsigned long SSN;
		m_Set.get(i, SSN);
		std::cout << SSN << '\n';
	}
}
	// Write every SSN in the SSNSet to cout exactly once, one per
	// line.  Write no other text.

