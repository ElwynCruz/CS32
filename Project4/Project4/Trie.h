#include <vector>



const int A = 0, C = 1, G = 2, T = 3;

template <class ValueType>
class Trie
{
public:
	Trie()
	{

	}
	~Trie();
	void reset();
	void insert(const std::string& key, const	ValueType& value);
	std::vector<ValueType> find(const std::string&	key, bool	exactMatchOnly)	const;

private:
	Node* root;

	struct Node
	{
		vector<ValueType> values;
		Child* children[4];

		struct Child
		{
			Node* next;
			int label; 
		};
	};
};
