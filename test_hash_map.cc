//Salem Saboori (Navid)
//Salem.Saboori00@myhunter.cuny.edu
//Hunter College - CSCI 335
//Prof. Stamos

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>



#include "quadratic_probing2.h"
#include "double_Probing3.h"

using namespace std;
bool oneCharoff(const string &word1, const string &word2) {
  if (word1.length() != word2.length()) {
    return false;
  }
  int diffs = 0;
  for (int i = 0; i < word1.length(); ++i) {
    if (word1[i] != word2[i]) {
      if (++diffs > 1) {
        return false;
      }
    }
  }
  return diffs == 1;
}


template <typename HashTableType>

void TestComputeAdjacentWords(HashTableType &hash_table,
                              const vector<string> &words) {
  // This is for computing timing.
  const auto begin = chrono::high_resolution_clock::now();

  hash_table.MakeEmpty();
  //..Insert your own code using algorithm described in Figure 4.73
  string word1, word2;
  size_t longest;
  vector<string>wordByLength;
  vector<vector<string>>wordLength;

  for (auto &str : words) {
    if (longest < str.length())
      longest = str.length();
  }

  for (size_t i = 0; i <= longest; ++i) {
    for (auto & str : words) {
      if (str.length() == i) {
        wordByLength.push_back(str);
      }
    }
    wordLength.push_back(wordByLength);
    wordByLength.clear();

  }
  for (auto & entry : wordLength) {
    const vector<string> &groupWords = entry;

    for (int i = 0; i < groupWords.size(); ++i) {
      for (int j = i + 1; j < groupWords.size(); ++j) {
        if (oneCharoff(groupWords[i], groupWords[j])) {
          hash_table.Insert(groupWords[i], groupWords[j]);
          hash_table.Insert(groupWords[j], groupWords[i]);
        }
      }
    }
  }



const auto end = chrono::high_resolution_clock::now();

cout << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << "ns" << endl;
cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;
cout<< "Please enter a word: "<<endl;
string firstWord, secondWord;
cin>>firstWord;
hash_table.check(firstWord);
cout<<" Please enter another word: "<<endl;
cin>>secondWord;
hash_table.check(secondWord);
}
vector<string> ReadWordsFromFile(const string &words_filename) {
  vector<string> the_words;
  // Implement this...
  ifstream wordsFile(words_filename); // read lines consisting of wordsFile in words_filename
  string input; // string that takes in word from words.txt
  if (!wordsFile.is_open()) {
    cerr << words_filename << " " << "Can not be open" << endl;
  } else {
    while (!wordsFile.eof()) {
      getline( wordsFile, input); { // while(wordsFile >> input)
        if ( input.length() < 1 ) { continue; } // If bad/empty lines
        if ( input.length() > 0 ) { the_words.push_back(input); }
      }
    }
  }
  wordsFile.close();
  return the_words;
}

// Sample main for program CreateAndTestHash
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " words_file_name hash_type (quadratic or double)" << endl;
    return 0;
  }

  const string words_filename(argv[1]);
  const vector<string> the_words = ReadWordsFromFile(words_filename);

  const string param_flag(argv[2]);

  if (param_flag == "quadratic") {
    HashQuadraticProbing<string> quadratic_probing_table;
    TestComputeAdjacentWords(quadratic_probing_table, the_words);
  } else if (param_flag == "double") {
    HashDoubleProbing<string> double_probing_table;
    TestComputeAdjacentWords(double_probing_table, the_words);
  }

  else {
    cout << "Uknown hash type " << param_flag << " (User should provide quadratic, or double)" << endl;
  }
  return 0;
}
