#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;


class GenomeMatcherImpl
{
public:
  GenomeMatcherImpl(int minSearchLength);
  void addGenome(const Genome& genome);
  int minimumSearchLength() const;
  bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
  bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
  
  
private:

  struct DNAStrand
  {
    //Genome* m_genome;// genome this strand of DNA belongs to
    int position;
    int genomeIndex;
    
  };
  Trie<DNAStrand> m_dnaSequences;
  int m_minSearchLength;
  vector<Genome> m_genomes;

  int findGenomesWithThisDNAHelper(const string& fragment, const string& other, bool exactMatchOnly) const;
};



GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
  :m_dnaSequences()
{
  m_minSearchLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
  m_genomes.push_back(genome);

  
  
  for (int i = 0; i + m_minSearchLength < genome.length(); i++)
    {
      string fragment;
      DNAStrand newDNA;
      newDNA.position = i;
      newDNA.genomeIndex = m_genomes.size() - 1;
      //newDNA->m_genome = genome;// point to the last genome we've added
      if (genome.extract(newDNA.position, m_minSearchLength, fragment))
	{
	  m_dnaSequences.insert(fragment, newDNA);
	  
	}
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
  return m_minSearchLength; 
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
  if (fragment.size() < minimumLength || minimumLength < m_minSearchLength)
    return false;
  vector<DNAStrand> possibleGenomes = m_dnaSequences.find(fragment.substr(0, m_minSearchLength) , exactMatchOnly);
  map<int, DNAMatch> longestMatches;//genomeIndex to DNAMatch
  
  DNAMatch newDNA;
  for (unsigned i = 0; i < possibleGenomes.size(); i++)
    {
      string match;
      DNAStrand cur = possibleGenomes[i];
      m_genomes[cur.genomeIndex].extract(cur.position, fragment.size(), match);
      int length = findGenomesWithThisDNAHelper(fragment, match, exactMatchOnly);
      if (longestMatches.find(cur.genomeIndex) == longestMatches.end())// not in our map yet
	{
	  newDNA.genomeName = m_genomes[cur.genomeIndex].name();
	  newDNA.length = length;
	  newDNA.position = cur. position;
	  longestMatches[cur.genomeIndex] = newDNA;
	}
      else
	{
	  if (longestMatches[cur.genomeIndex].length < length)// same genome, check length
	    {
	      longestMatches[cur.genomeIndex].position = cur.position;
	      longestMatches[cur.genomeIndex].length = length;
	      
	    }
	  else if (longestMatches[cur.genomeIndex].length == length)
	    {
	      longestMatches[cur.genomeIndex].position = min(longestMatches[cur.genomeIndex].position, cur.position);
	    }
	}
    }

  map<int, DNAMatch>::iterator it = longestMatches.begin();
  matches.clear();
  while (it != longestMatches.end())
    {
      if (it->second.length >= minimumLength)
	matches.push_back(it->second);
      it++;
    }

  return (matches.size() != 0);
}

int GenomeMatcherImpl::findGenomesWithThisDNAHelper(const string& fragment, const string& other, bool exactMatchOnly) const// examine two strings recursively, returns position of second mismatch or length of fragment, whichever is first
{
  if (fragment == "" || other == "")
    return 0;
  else if (fragment[0] != other[0] && exactMatchOnly)
    return 0;
  else if (fragment[0] != other[0])
    return 1 + findGenomesWithThisDNAHelper(fragment.substr(1), other.substr(1), true);
  else 
    return 1 + findGenomesWithThisDNAHelper(fragment.substr(1), other.substr(1), exactMatchOnly);
}


bool compGenomeMatches(GenomeMatch first, GenomeMatch second)// highest percentMatch goes first, if tie, then alphabetically goes first
{
  if (first.percentMatch > second.percentMatch)
    return true;
  else if (first.percentMatch < second.percentMatch)
    return false;
  else
    return (first.genomeName < second.genomeName);
}



bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{

  if (fragmentMatchLength < m_minSearchLength)
    return false;
  int numSequences = query.length() / fragmentMatchLength;
  
  map<string, int> numMatches; // map each Genome to number of times it's been seen outline:
  for (int i = 0; i < numSequences; i++)//  Use DNAMatches from findGenomesWithThisDNA
    {// to map each DNAMatch's genome_name to the number of times it has matched
      string fragment;// then iterate through map and for each entry, divide numMatches by numSequences
      if (query.extract(i*numSequences, fragmentMatchLength, fragment))// store answer and genome_name in GenomeMatch variable and push onto results
	{
	  vector<DNAMatch> matches;
	  findGenomesWithThisDNA(fragment, fragmentMatchLength, exactMatchOnly, matches);
	    
	      for (int k = 0; k < matches.size(); k++)
		{
		  if (numMatches.find(matches[k].genomeName) != numMatches.end())
		    numMatches[matches[k].genomeName] += 1;// found another, add to the total
		  else
		    numMatches[matches[k].genomeName] = 1;// first one we found
		}
	    
	}
    }

  map<string, int>::iterator it = numMatches.begin();
  while (it != numMatches.end())
    {
      double percentMatch = it->second / numSequences * 100;
      if (percentMatch >= matchPercentThreshold)
	{
	  GenomeMatch gm;
	  gm.genomeName = it->first;
	  gm.percentMatch = percentMatch;
	  results.push_back(gm);
	}
      
    }
  sort(results.begin(), results.end(), compGenomeMatches);
  return results.size() != 0;  // This compiles, but may not be correct
}



//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
  m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
  delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
  m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
  return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
  return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
  return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
/*
int main()
{
const string providedFiles[] = {
"Halorubrum_californiense.txt",
"test2.txt",
"test.txt"
//"Ferroplasma_acidarmanus.txt"
"Halobacterium_jilantaiense.txt",
"Halorubrum_chaoviator.txt",
"Halorubrum_californiense.txt",
"Halorientalis_regularis.txt",
"Halorientalis_persicus.txt",
"Ferroglobus_placidus.txt",
"Desulfurococcus_mucosus.txt"
};
GenomeMatcherImpl gm(10);
for (const string& fileName : providedFiles)
{
ifstream strm("C:/Users/elwyn/Documents/School/CS32/Project4/Gee-nomics/data/" + fileName);
vector<Genome>vg;
boolsuccess = Genome::load(strm, vg);//Loadthedataviathestream.

if (success)
{
cout << "Loaded " << vg.size() << " genomes successfully from " << fileName << " "<< endl;
for (int k = 0; k != vg.size(); k++)
gm.addGenome(vg[k]);

}
else
{
cout << "Fail" << endl;
}
}
vector<DNAMatch> matches;
//vector<DNAStrand> help;
//Trie<DNAStrand> help;
gm.findGenomesWithThisDNA("ACGAATCACGTGCGAGA", 11, true, matches);
//gm.findGenomesWithThisDNA("GAAG", 4, true, matches);
//gm.findGenomesWithThisDNA("GAATACG", 6, false, matches);
//help = gm.m_dnaSequences.find("A", true);
string fragment;
for (int k = 0; k != matches.size(); k++)
{
cout << matches[k].genomeName << " is length ";
cout << matches[k].length << " at position " << matches[k].position << endl;
}
}
*/
