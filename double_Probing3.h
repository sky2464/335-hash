#ifndef DOUBLE_PROBING3_H
#define DOUBLE_PROBING3_H

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

#include "is_prime.h"

using namespace std;

template <typename HashedObj>
class HashDoubleProbing {
public:
	enum EntryType {ACTIVE, EMPTY, DELETED};

	explicit HashDoubleProbing(size_t size = 101000) : array_(PrimeProject::NextPrime(size))
    { MakeEmpty(); }

    void check(const HashedObj &x)
  {
    size_t current_pos=FindPos(x);
    cout<<"the lists are : "<<endl;
    for(auto& lol:array_[current_pos].values)
      cout<<lol<<endl;
  }  

    bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
	}

    void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;

    for(size_t i= array_.size();i>0;--i)
    {
      if(PrimeProject::IsPrime(i))
      {
        R=i;
        //cout<<R<<endl;

      }
    }
  	}
    bool Insert(const HashedObj & x,const HashedObj & y) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    
    array_[current_pos].element_ = x;
    array_[current_pos].values.push_back(y);
    array_[current_pos].info_ = ACTIVE;
    
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
    }

  	bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  
  }
  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }


private:
	struct HashEntry {
    HashedObj element_;
    vector<HashedObj>values;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };

    std::vector<HashEntry> array_;
    size_t current_size_;
    size_t R;

  	bool IsActive(size_t current_pos) const
  	{ return array_[current_pos].info_ == ACTIVE; }
  	
  	size_t FindPos(const HashedObj & x) const {
    size_t current_pos = First_Hash(x);
    size_t offset = Second_Hash(x);
      
    while (array_[current_pos].info_ != EMPTY &&array_[current_pos].element_ != x)
    {
      	current_pos += offset;  // Compute ith probe.
      	if (current_pos >= array_.size())
      	{
			     current_pos -= array_.size();
      	}
    }
    return current_pos;
 	}

    void Rehash() {
    std::vector<HashEntry> old_array = array_;
    int old_size=current_size_;

    // Create new double-sized, empty table.
    array_.resize(PrimeProject::NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    //cout<<current_size_<<endl;
    // Copy table over.
    current_size_=0;
    for(auto &this_array:old_array)
    {
      array_.push_back(this_array);
    }
    current_size_=old_size;


  }

	size_t First_Hash(const HashedObj & x) const {
    	static std::hash<HashedObj> hf;
    	return hf(x) % array_.size( );
	}
	size_t Second_Hash(const HashedObj & x) const {
		static std::hash<HashedObj> hf;
		//R should be the prime number smaller than the table size.
		return (R - (hf(x) % R));
	}

 };

 #endif  // DOUBLE_PROBING3_H