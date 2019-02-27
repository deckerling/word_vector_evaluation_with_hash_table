// wvewht.h

// Copyright 2019 E. Decker
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WORD_VECTOR_EVALUATION_WITH_HASH_TABLE_SRC_WVEWHT_H_ // wvewht = "word_vector_evaluation_with_hash_table"
#define WORD_VECTOR_EVALUATION_WITH_HASH_TABLE_SRC_WVEWHT_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class HashTable;

class HashTableReader {
// Class to read hash tables created by "HashTableWriter".
 public:
  HashTableReader(const std::string &hash_table_file);
  ~HashTableReader();
  void CompareWordVectors(const std::vector<std::string> &words);
  void GetVectors(HashTable &hash_table, const std::vector<std::string> &words);

 private:
  const std::string hash_table_file_;
  const std::vector<int> hash_table_values_;
  std::vector<int> GetHashTableValues();
  void GetBothVectors(const std::string &line, const std::vector<std::string> &words_to_find, std::vector<std::vector<double>> &vectors);
  std::string GetWordVectorsFromLine(const std::string &line, const std::string &word_to_find);
  std::vector<double> GetVector(const std::string &word_vector);

  bool CheckIndex(const std::string &index, const std::string &line) {
  // Checks if the current "line" of a hash table file is equal to the "index"
  // of the bucket in question and returns "true" if that is the case and
  // "false" otherwise.
    return (index == line.substr(0, line.find_first_of(',')));
  }

  bool VectorIsValid(const std::string &word, const std::vector<double> &vector_to_check) {
  // Checks whether "vector_to_check" contains only zeros - if so, "false" will
  // be returned (for this means that the corresponding "word" was not found);
  // otherwise "true" will be returned.
    if (vector_to_check != std::vector<double>(hash_table_values_[0], 0))
      return true;
    else {
      std::cout << "\t\"" << word << "\" couldn't be found in your data! Comparison impossible.\n" << std::endl;
      return false;
    }
  }
};

class HashTable {
// Basic hash table class containing the main methods and members of a hash
// table.
 public:
  HashTable(const std::string &input_file);
  HashTable(const int hash_table_size);
  ~HashTable();

  bool HashTableIsValid() {
    return (vector_size_ < 1 || vector_num_ < 1)? false : true;
  }

 protected:
  const std::string input_file_;
  const int vector_size_, vector_num_, hash_table_size_;
  const int GetSizeOfVectors();
  const int CountVectors();
  int GetIndex(const std::string &key); // hash function
  void ShowInfo(const int num_of_empty_buckets, const int highest_num_of_items_in_a_bucket);
  void ShowSimilarity(const std::vector<std::string> &words, const std::vector<std::vector<double>> &vectors);
  double CalculateCosineSimilarity(const std::vector<std::vector<double>> &vectors);
  double CalculateEuclideanDistance(const std::vector<std::vector<double>> &vectors);
  double CalculateEuclideanNorm(const std::vector<double> &vector);

 friend void HashTableReader::GetVectors(HashTable &hash_table, const std::vector<std::string> &words);
};

class HashTableOnMemory : public HashTable {
// Class to create a hash table on memory containing the word vectors of a
// given word vector file.
 public:
  HashTableOnMemory(const std::string &file);
  ~HashTableOnMemory();
  void PrintInfo();
  void CompareWordVectors(const std::vector<std::string> &words);

 private:
  struct WordVector {
    std::string word;
    std::vector<double> vector;
    WordVector *next = NULL;
  };
  std::vector<WordVector*> hash_table_;
  void ReadVectorFile();
  void StoreVectors(const std::string &line);
  std::vector<std::string> SplitLine(const std::string &line);
  int GetNumOfWordVectors(const int index);
  std::vector<double> GetVector(const std::string &word);
};

class HashTableWriter : public HashTable {
// Class to create a hash table containing the word vectors of a given word
// vector file and to write this hash table to a file.
 public:
  HashTableWriter(const std::string &input_file, const std::string &output_file);
  ~HashTableWriter();

 private:
  const std::string input_file_, output_file_;
  void CreateHashTable();

  bool VectorInCurrentBucket(const std::string &line, const int bucket_num) {
  // Returns "true" if the word vector fits into the current bucket and "false"
  // if not. (Remember: If the vector file is designed in a way this program
  // can work with, the "word" of a word vector is equal to all of the chars of
  // a line up to the first whitespace (i.e. the first "item" of a line)).
    return (bucket_num == GetIndex(line.substr(0, line.find_first_of(' '))));
  }
};

class BinaryTree {
 public:
  BinaryTree();
  ~BinaryTree();
  void AddIndex(const int index_to_add);
  bool IndexIsAlreadyStored(const int index_to_check_for);

 private:
  struct Item {
    int index = -1;
    struct Item *left = NULL;
    struct Item *right = NULL;
  };
  struct Item *root_ = NULL;
  void DestroyTree(struct Item *current_item);
};

#endif // WORD_VECTOR_EVALUATION_WITH_HASH_TABLE_SRC_WVEWHT_H_
