// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>  
using namespace std;

void removeNonLetters(string& s);

class DictionaryImpl
{
public:
    DictionaryImpl(int maxBuckets) { hashTable.resize(maxBuckets); hash_buckets = maxBuckets; }
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string)) const; // change back to const later
    unsigned int hash_function(const string& hashMe) const;
private:
    int hash_buckets;
    struct Bucket
    {
        Bucket(string s) : word(s), word_sorted(s) { std::sort(word_sorted.begin(), word_sorted.end()); }
		string word;
        string word_sorted;
    };    
    vector<list<Bucket>> hashTable; 
};

unsigned int DictionaryImpl::hash_function(const string& hashMe) const
{
	hash<string> str_hash;
    size_t hashValue = str_hash(hashMe);
	unsigned int numOfBuckets = hashValue % hash_buckets;
    return numOfBuckets;
}

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
	Bucket temp(word);
    int bucket = hash_function(temp.word_sorted);
    
	hashTable[bucket].push_back(Bucket(temp));
	return;
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
    {
        return;
    }
	removeNonLetters(letters);
    Bucket temp(letters);
    int bucket = hash_function(temp.word_sorted);
    if (hashTable[bucket].empty())
    {
        return;
    }
    list<Bucket>::const_iterator it = hashTable[bucket].begin();
	for (; it != hashTable[bucket].end(); it++)
    {
        if (it->word.length() == temp.word.length())
        {
            if (it->word_sorted == temp.word_sorted)
            {
                callback(it->word);
            }
        }
    }
    return;
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const 
{
    m_impl->lookup(letters, callback);
}