#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>


template<typename ValueType>
class Trie
{
 public:
  Trie()
    {
      root = new Node;
    }
  ~Trie()
    {
      resetHelper(root);
    }
  void reset()
  {
    resetHelper(root);
    root = new Node;

    
  }

  void insert(const std::string& key, const ValueType& value)
  {
    Node* curNode = root;
    for (int i = 0; i < key.size(); i++)
      {
	bool foundChild = false;
	for (unsigned j = 0; j < curNode->children.size(); j++)
	  {
	    if (curNode->children[j]->label == key[i])
	      {
		curNode = curNode->children[j];
		foundChild = true;
		break;
	      }
	  }
	
	if (!foundChild) // didn't find value in children
	  {
	    Node* newChild = new Node;
	    newChild->label = key[i];
	    curNode->children.push_back(newChild);
	    curNode = newChild;
	  }
	
      }
    curNode->values.push_back(value);
  }


  std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const
    {
      Node* curNode = root;
      std::vector<ValueType> matches;
      for (unsigned i = 0; i < curNode->children.size(); i++)
	{
	  std::vector<ValueType> newMatches;
	  if (curNode->children[i]->label == key[0])
	    {
	      newMatches = findHelper(key.substr(1), exactMatchOnly, curNode->children[i]);
	    }
	  matches.insert(matches.end(), newMatches.begin(), newMatches.end());
	}
      return matches;
    }

  // C++11 syntax for preventing copying and assignment
  Trie(const Trie&) = delete;
  Trie& operator=(const Trie&) = delete;
 private:
  struct Node
  {
    std::vector<ValueType> values;
    std::vector<Node*> children;
    char label;
  };
  
  Node* root;
  void resetHelper(Node* cur)// recursively deletes all the nodes
  {
    if (cur == nullptr)
      return;

    for (unsigned i = 0; i < cur->children.size(); i++)
      {
	resetHelper(cur->children[i]);
      }
    delete cur;
  }
  std::vector<ValueType> findHelper(const std::string& key, bool exactMatchOnly, Node* current) const// recursively find node 
    {
      std::vector<ValueType> matches;
      if (key == "")// made it to the end
	return current->values;
      for (unsigned i = 0; i < current->children.size(); i++)
	{
	  std::vector<ValueType> newMatches;
	  if (current->children[i]->label == key[0])
	    {
	      newMatches = findHelper(key.substr(1), exactMatchOnly, current->children[i]);
	    }
	  else if (!exactMatchOnly)// we can still have 1 mismatch
	    {
	      newMatches = findHelper(key.substr(1), true, current->children[i]);// no more mismatches allowed
	    }

	  matches.insert(matches.end(), newMatches.begin(), newMatches.end());
	}
      return matches;

    }
};

/*
#include <string>
#include <vector>


template<typename ValueType>
class Trie
{
public:
Trie()
{
root = new Node;
}
~Trie()
{
resetHelper(root);
}
void reset()
{
resetHelper(root);
root = new Node;


}

void insert(const std::string& key, const ValueType& value)
{
Node* curNode = root;
for (int i = 0; i < key.size(); i++)
  {
if (curNode->children[key[i]] == nullptr)
  {
Node* newChild = new Node;
curNode->children[key[i]] = newChild;
curNode = newChild;
}
 else
   curNode = curNode->children[key[i]];

}
curNode->values.push_back(value);
}


std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const
{
Node* curNode = root;
std::vector<ValueType> matches;
for (unsigned i = 0; i < key.size(); i++)
  {
std::vector<ValueType> newMatches;
if (curNode->children[key[0]] != nullptr)
  {
newMatches = find(key.substr(1), exactMatchOnly);
}
 else
   {
if (exactMatchOnly)
  return matches;
 else
   {
if (curNode->children[i] != nullptr)
  newMatches = find(key.substr(1), true);
}
}
matches.insert(matches.end(), newMatches.begin(), newMatches.end());
}
return matches;
}

// C++11 syntax for preventing copying and assignment
Trie(const Trie&) = delete;
Trie& operator=(const Trie&) = delete;
private:
struct Node
{
std::vector<ValueType> values;
std::vector<Node*> children;
Node()
:children(128, nullptr)
{

}

};

Node* root;
void resetHelper(Node* cur)// recursively deletes all the nodes
{
if (cur == nullptr)
  return;

for (unsigned i = 0; i < cur->children.size(); i++)
  {
resetHelper(cur->children[i]);
}
delete cur;
}
};


*/
#endif // TRIE_INCLUDED
