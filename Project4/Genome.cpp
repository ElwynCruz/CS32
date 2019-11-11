#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
  GenomeImpl(const string& nm, const string& sequence);
  static bool load(istream& genomeSource, vector<Genome>& genomes);
  int length() const;
  string name() const;
  bool extract(int position, int length, string& fragment) const;
private:
  string m_name;
  string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
  m_name = nm;
  m_sequence = sequence;

}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)// does not read last genome (add this later)
{
  string line;
  string name = "";
  string sequence = "";
  bool hasName = false;
  bool hasSequence = false;
  while (getline(genomeSource, line) || genomeSource.eof())
    {
      if (genomeSource.eof())
	{
	  if (hasName)
	    {
	      if (!hasSequence)
		return false;
	      Genome DNA(name, sequence);
	      genomes.push_back(DNA);
	      return true;
	    }
	  return false;
	}

      if (line.empty())
	return false;



      if (line[0] == '>')
	{
	  if (hasName)
	    {
	      if (!hasSequence)
		return false;

	      Genome DNA(name, sequence);
	      genomes.push_back(DNA);
	      hasName = false;// push Genome onto vector, reset everything
	      hasSequence = false;
	      sequence = "";
	      name = "";
	    }


	  name = line.substr(1);// exclude first and last character (> and \n)
	  hasName = true;
	  if (name.size() == 0)
	    return false;
	}
      else
	{
	  if (!hasName)
	    return false;

	  for (int i = 0; i < line.size(); i++)
	    {
	      switch (toupper(line[i]))
		{
		case 'A':
		case 'G':
		case 'T':
		case 'C':
		case 'N':
		  sequence += toupper(line[i]);
		  break;
		default:
		  return false;
		}
	      hasSequence = true;
	    }
	}
    }



  return true;
}

int GenomeImpl::length() const
{
  return m_sequence.size();
}
string GenomeImpl::name() const
{
  return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
  if (position + length > this->length())
    {
      return false;
    }

  fragment = m_sequence.substr(position, length);


  return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
  m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
  delete m_impl;
}

Genome::Genome(const Genome& other)
{
  m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
  GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
  delete m_impl;
  m_impl = newImpl;
  return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
  return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
  return m_impl->length();
}

string Genome::name() const
{
  return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
  return m_impl->extract(position, length, fragment);
}
