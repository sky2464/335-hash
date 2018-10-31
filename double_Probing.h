//Salem Saboori "Navid"
//Salem.Saboori00@myhunter.cuny.edu

#ifndef DOUBLE_PROBING_H
#define DOUBLE_PROBING_H


#include <iostream>
#include <vector>
#include <algorithm>
#include "is_prime.h"
#include <functional>
#include <string>
using namespace std;

/* Double Probing class. Hash function hash1 : x % sizeofTable // Default sizeofTable = 101
 * If occupied, 2nd hashing. h(x): (R - (hash1(x) % R) position away from 1st hash */

template <typename HashedObj>
class DoubleHashTable{
public:
	explicit DoubleHashTable(int size = 101) : array(PrimeProject::NextPrime(size))// An empty hash table
	{ MakeEmpty(); }

  enum EntryType {ACTIVE, EMPTY, DELETED};

	void MakeEmpty()	{ // remove all items
		currentSize = 0;
		for (auto & entry : array) entry.info = EMPTY;
	}

	bool Insert(const HashedObj & x, HashedObj &y)	{ // Insert x as active - HashObj (constant)
		size_t currentPos = FindPos(x);
		if (isActive(currentPos))
			return false;

		array[currentPos].element = x;
		array[currentPos].info = ACTIVE;
		array[currentPos].element = y;

		// Rehash; see Section 5.5
		if (++currentSize > array.size() / 2)
			rehash();

		return true;
	}

	bool Insert(HashedObj && x, HashedObj && y) {	// Insert x as active
		size_t currentPos = FindPos(x); // int current_pos = FindPos(x);
		if (isActive(currentPos))
			return false;

		array[currentPos] = std::move(x);
		array[currentPos].info = ACTIVE;
		array[currentPos] = std::move(y);

		// Rehash; see Section 5.5
		if (++currentSize > array.size() / 2)
			rehash();

		return true;
	}

  bool Contains(const HashedObj & x) const { return isActive(FindPos(x)); }

	bool Remove(const HashedObj & x)	{
		size_t currentPos = FindPos(x);
		if (!isActive(currentPos))
			return false;

		array[currentPos].info = DELETED;
		return true;
	}

	
private:
	struct HashEntry {
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj{}, EntryType i = EMPTY)
			: element{ e }, info{ i } { }

		HashEntry(HashedObj && e, EntryType i = EMPTY)
			: element{ std::move(e) }, info{ i } { }
	};
	int R = 83; // R is a choice for internalhash2, i.e prime # < tableSize (101)
	vector<HashEntry> array;
	size_t currentSize;
	

	bool isActive(int currentPos) const { return array[currentPos].info == ACTIVE;	}

	size_t InternalHash(const HashedObj & x) const	{ // Hash from quadratic probing
		static hash<HashedObj> hf;
		return hf(x) % array.size();
	}

	size_t InternalHash2(const HashedObj & x) const	{ // 2nd hash for double Probing
		static hash<HashedObj> hf;
		return R - (hf(x) % R);
	}

	size_t FindPos(const HashedObj & x) const	{ // Sequence of probes:	Probe(i) = (hash(x) + i * hash2(x)) mod T
  		size_t firstPos = InternalHash(x);
		int i = 0;
		size_t probe = firstPos; // 2nd hash
		while (array[probe].info != EMPTY &&	array[probe].element != x)	{
			
			++i;
			probe = (firstPos + (i * InternalHash2(x))) % array.capacity(); // Compute the ith double probe
		}
		return probe;
	}

	void rehash() {	// Rehash if size exceeds 101
		vector<HashEntry> oldArray = array;

		// Create new double-sized, empty table
		array.resize(nextPrime(2 * oldArray.size()));
		for (auto & entry : array)
			entry.info = EMPTY;

		// Copy table over
		currentSize = 0;
		for (auto & entry : oldArray)
			if (entry.info == ACTIVE)	Insert(std::move(entry.element));
	}
};
#endif //DOUBLE_PROBING_H


