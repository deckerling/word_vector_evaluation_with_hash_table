// hash_tables.cc

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

#include <algorithm>
#include <fstream>
#include <math.h>
#include <numeric>

#include "wvewht.h" // wvewht = "word_vector_evaluation_with_hash_table"

// The standard constructor of the "HashTable"
HashTable::HashTable(const std::string &input_file)
    : input_file_(input_file),
      vector_size_(GetSizeOfVectors()),
      vector_num_(CountVectors()),
      hash_table_size_(vector_num_/20) {} // in some cases you may have to adjust the denominator in order to reduce the number of collisions

// The constructor "HashTableReader" will use.
HashTable::HashTable(const int hash_table_size)
    : vector_size_(0), // default initialization of "vector_size_" and "vector_num_" for they are not needed if "HashTableReader" is active
      vector_num_(0),
      hash_table_size_(hash_table_size) {}

HashTable::~HashTable() {}

const int HashTable::GetSizeOfVectors() {
// Returns the number of dimensions of the word vectors found in "input_file_"
// (assuming that each line of the file contains exactly one vector and that
// all the word vectors got the same number of dimensions).
  std::ifstream file_stream(input_file_);
  if (!file_stream.is_open()) {
    std::cout << "ERROR: OPENING \"" << input_file_ << "\" FAILED!\nMake sure that the file exists and that the path is correct.\n";
    std::cout << "Program terminated.";
    return -1;
  } else if (file_stream.bad()) {
    std::cout << "ERROR: OPENING \"" << input_file_ << "\" FAILED!\n";
    std::cout << "Program terminated.";
    return -1;
  }
  std::cout << "Input file (\"word vector file\"): " << input_file_ << '\n';
  std::cout << "\tChecking the size of the word vectors..." << std::endl;
  std::string line;
  std::getline(file_stream, line);
  file_stream.close();
  std::cout << "\t---Done.\n";
  return (std::count(line.begin(), line.end(), ' '));
}

const int HashTable::CountVectors() {
// Returns the number of word vectors in "input_file_" (assuming that each line
// of the file contains exactly one vector).
  if (vector_size_ < 1)
    return -1;
  int vector_num = 0;
  std::ifstream file_stream(input_file_);
  std::cout << "\tCounting the word vectors..." << std::endl;
  std::string line;
  while (std::getline(file_stream, line))
    vector_num++; // this might cause problems if your "input_file_" is not a valid word vector file because actually lines and not vectors are counted
  file_stream.close();
  std::cout << "\t---Done." << std::endl;
  return vector_num;
}

int HashTable::GetIndex(const std::string &key) { // hash function
// Returns the "index" of the bucket of the hash table the "key" corresponds to.
  int hash = 0, j = 1, k = 0;
  const std::vector<int> primes = {179, 181, 191, 193, 197, 199, 211, 223, 227, 229};
  for (unsigned i = 0; i < key.length(); ++i) {
    if (i == (primes.size()*j)) {
      k = 0;
      j++;
    }
    hash += (int) key[i]*primes[k]; // multiplies each ASCII-value of "key" with an element of "primes" (i.e. a prime number)
    k++;
  }
  const int index = hash%hash_table_size_;
  if (index < 0)
    return 0;
  if (index > (hash_table_size_-1))
    return hash_table_size_-1;
  return index;
}

void HashTable::ShowInfo(const int num_of_empty_buckets, const int highest_num_of_items_in_a_bucket) {
// Prints the most important information regarding the created hash table.
  std::cout << "\tSize of vectors = " << vector_size_ << '\n';
  std::cout << "\tNumber of stored word vectors = " << vector_num_ << '\n';
  std::cout << "\tNumber of buckets = " << hash_table_size_ << '\n';
  std::cout << "\tLoad factor = " << (double) vector_num_/hash_table_size_ << '\n';
  std::cout << "\tNumber of empty buckets = " << num_of_empty_buckets << '\n';
  std::cout << "\tPercentage of empty buckets = " << 100*((double) num_of_empty_buckets/hash_table_size_) << " %\n";
  std::cout << "\tHighest number of word vectors in a bucket = " << highest_num_of_items_in_a_bucket << '\n';
  std::cout << "\tPercentage of vectors in mostly filled bucket = " << 100*((double) highest_num_of_items_in_a_bucket/vector_num_) << '\n';
}

void HashTable::ShowSimilarity(const std::vector<std::string> &words, const std::vector<std::vector<double>> &vectors) {
// Prints the cosine similarity and the Euclidean distance of two word vectors.
  std::cout << "\tThe cosine similarity of the word vectors of \"" << words[0] << "\" and \"" << words[1] << "\" =\n\t " << CalculateCosineSimilarity(vectors) << '\n';
  std::cout << "\tThe Euclidean distance between the word vectors of \"" << words[0] << "\" and \"" << words[1] << "\" =\n\t " << CalculateEuclideanDistance(vectors) << "\n\n";
}

double HashTable::CalculateCosineSimilarity(const std::vector<std::vector<double>> &vectors) {
// Calculates and returns the cosine similarity of "vectors[0]" and
// "vectors[1]".
  return ((inner_product(std::begin(vectors[0]), std::end(vectors[0]), std::begin(vectors[1]), 0.))/(CalculateEuclideanNorm(vectors[0])*CalculateEuclideanNorm(vectors[1])));
}

double HashTable::CalculateEuclideanDistance(const std::vector<std::vector<double>> &vectors) {
// Calculates and returns the Euclidean distance between "vectors[0]" and
// "vectors[1]".
  double x = 0;
  for (unsigned i = 0; i < vectors[0].size(); ++i)
    x += std::pow((vectors[0][i]-vectors[1][i]), 2);
  return std::sqrt(x);
}

double HashTable::CalculateEuclideanNorm(const std::vector<double> &vector) {
// Calculates and returns the Euclidean norm of "vec" (needed in order to
// calculate the cosine similarity).
  double x = 0;
  for (unsigned i = 0; i < vector.size(); ++i)
    x += std::pow(vector[i], 2);
  return std::sqrt(x);
}

HashTableOnMemory::HashTableOnMemory(const std::string &input_file)
    : HashTable(input_file) {
  std::vector<struct WordVector*> HT(hash_table_size_);
  hash_table_ = HT;
  ReadVectorFile();
}

HashTableOnMemory::~HashTableOnMemory() {
  struct WordVector *current_word_vector;
  for (int i = 0; i < hash_table_size_; ++i) {
    while (hash_table_[i] != NULL) {
      current_word_vector = hash_table_[i];
      hash_table_[i] = hash_table_[i]->next;
      delete current_word_vector;
    }
  }
}

void HashTableOnMemory::ReadVectorFile() {
// Reads "input_file_" and passes the lines to
// "HashTableOnMemory::store_vectors()".
  if (!HashTableIsValid())
    return;
  std::cout << "\tLoading data..." << std::endl;
  std::string line;
  std::ifstream vector_file_stream(input_file_);
  while (std::getline(vector_file_stream, line))
    StoreVectors(line);
  std::cout << "\t---Completed.\n";
}

void HashTableOnMemory::StoreVectors(const std::string &line) {
// Stores the word vectors in the hash table on memory.
  const std::vector<std::string> tokens = SplitLine(line);
  std::vector<double> vector(vector_size_);
  const int index = GetIndex(tokens[0]);
  if (hash_table_[index] == NULL) {
    hash_table_[index] = new struct WordVector;
    hash_table_[index]->word = tokens[0];
    for (int i = 0; i < vector_size_; ++i)
      // Converts the (std::string) elements of "tokens" that represent the
      // values of the word vector into the type "double".
      vector[i] = atof(tokens[i+1].c_str());
    hash_table_[index]->vector = vector;
  } else { // collisions are handled by chaining using a linked list
    struct WordVector *current_word_vector = hash_table_[index];
    struct WordVector *new_word_vector = new struct WordVector;
    new_word_vector->word = tokens[0];
    for (int i = 0; i < vector_size_; ++i)
      vector[i] = atof(tokens[i+1].c_str());
    new_word_vector->vector = vector;
    while (current_word_vector->next != NULL)
      current_word_vector = current_word_vector->next;
    current_word_vector->next = new_word_vector;
  }
}

std::vector<std::string> HashTableOnMemory::SplitLine(const std::string &line) {
// Splits the lines (strings) of the vector file into their tokens and returns
// all of those tokens in a std::vector. (The tokens within a "line" should be
// separated by whitespaces.)
  std::stringstream stream(line);
  std::string item;
  std::vector<std::string> tokens(vector_size_+1);
  for (unsigned i = 0; i < tokens.size(); ++i) {
    getline(stream, item, ' ');
    tokens[i] = item;
  }
  return tokens;
}

void HashTableOnMemory::PrintInfo() {
// Calculates some of the numeric information of the created hash table and
// passes them to "ShowInfo()" that prints them.
  int num_of_empty_buckets = 0, highest_num_of_items_in_a_bucket = 0, word_vector_num;
  for (int i = 0; i < hash_table_size_; ++i) {
    word_vector_num = GetNumOfWordVectors(i);
    if (word_vector_num == 0)
      num_of_empty_buckets++;
    if (highest_num_of_items_in_a_bucket < word_vector_num)
      highest_num_of_items_in_a_bucket = word_vector_num;
  }
  ShowInfo(num_of_empty_buckets, highest_num_of_items_in_a_bucket);
}

int HashTableOnMemory::GetNumOfWordVectors(const int index) {
// Counts the number of word vectors (items) in a bucket of the
// "HashTableOnMemory" and returns this number.
  int count = 0;
  if (hash_table_[index] == NULL)
    return 0;
  else {
    count++;
    struct WordVector *current_word_vector = hash_table_[index];
    while (current_word_vector->next != NULL) {
      count++;
      current_word_vector = current_word_vector->next;
    }
  }
  return count;
}

void HashTableOnMemory::CompareWordVectors(const std::vector<std::string> &words) {
// Starts searching for the word vectors corresponding to the "words" by
// passing the "words" to "GetVector()". If a word cannot be found in the
// "HashTableOnMemory" "GetVector()" returns a vector with every element == 0.
// It will be checked if the returned vector is equal to a "check vector"
// ("vectors[2]") that also contains only zeros. If so, the method stops by
// returning. If both word vectors are found, they will be passed to
// "ShowSimilarity()".
  std::vector<std::vector<double>> vectors(3, std::vector<double>(vector_size_));
  for (unsigned i = 0; i < words.size(); ++i) {
    vectors[i] = GetVector(words[i]);
    if (vectors[i] == vectors[2])
      return;
  }
  ShowSimilarity(words, vectors);
}

std::vector<double> HashTableOnMemory::GetVector(const std::string &word) {
// Given a word (std::string) this method returns the corresponding vector if
// the word and its vector are stored in the "HashTableOnMemory"; if not, a
// vector with every element == 0 will be returned.
  const int index = GetIndex(word);
  if (hash_table_[index] != NULL) {
    if (hash_table_[index]->word == word)
      return hash_table_[index]->vector;
    else {
      struct WordVector *current_word_vector = hash_table_[index];
      while (current_word_vector->next != NULL) {
        current_word_vector = current_word_vector->next;
        if (current_word_vector->word == word)
          return current_word_vector->vector;
      }
    }
  }
  std::cout << "\t\"" << word << "\" couldn't be found in your data! Comparison impossible.\n\n";
  return (std::vector<double> (vector_size_));
}

HashTableWriter::HashTableWriter(const std::string &input_file, const std::string &output_file)
    : HashTable(input_file),
      input_file_(input_file),
      output_file_(output_file) {
  CreateHashTable();
}

HashTableWriter::~HashTableWriter() {}

void HashTableWriter::CreateHashTable() {
// Creates a hash table containing the word vectors from the "input_file_" and
// saves it in the "output_file_".
  if (!HashTableIsValid())
    return;
  std::cout << "Output file (\"hash table file\"): " << output_file_ << '\n';
  std::cout << "\tCreating hash table file with " << hash_table_size_ << " buckets..." << std::endl;
  std::string vector_line, current_bucket;
  std::ofstream out;
  int num_of_empty_buckets = 0, num_of_items_in_current_bucket, highest_num_of_items_in_a_bucket, flush_interval_factor = 1;
  out.open(output_file_, std::ios_base::app);
  out << vector_size_ << ',' << vector_num_ << ',' << hash_table_size_ << '\n';
  BinaryTree indices;
  // All vectors of a bucket get connected to a single std::string, which will
  // be written to the hash table file.
  for (int bucket_num = 0; bucket_num < hash_table_size_; ++bucket_num) {
    num_of_items_in_current_bucket = 0;
    std::ifstream input_file_stream(input_file_);
    current_bucket = std::to_string(bucket_num);
    // Every word vector (i.e. every line of the "input_file_") gets checked if
    // it should go into the current bucket.
    for (int i = 0; i < vector_num_; ++i) {
      std::getline(input_file_stream, vector_line);
      // If the "vector_line" (i.e. the word vector) was either already added
      // to "current_bucket" or already written to the "output_file_" the line
      // will be skipped.
      if (!indices.IndexIsAlreadyStored(i) && VectorInCurrentBucket(vector_line, bucket_num)) {
        current_bucket += ','+vector_line; // collisions are handled by chaining
        num_of_items_in_current_bucket++;
        // The index of the line gets stored in a binary tree in order to skip
        // the line next time due to the condition of the if-statement (this
        // reduces the average running time).
        indices.AddIndex(i);
      }
    }
    input_file_stream.close();
    if (num_of_items_in_current_bucket > highest_num_of_items_in_a_bucket)
      highest_num_of_items_in_a_bucket = num_of_items_in_current_bucket;
    if (current_bucket != std::to_string(bucket_num))
      out << current_bucket << '\n';
    else
      num_of_empty_buckets++;
    if (bucket_num == (500*flush_interval_factor-1)) { // flushes in an interval of 500 (with respect to "bucket_num")
      out.flush();
      std::cout << '\t' << bucket_num+1 << " buckets ready..." << std::endl;
      flush_interval_factor++;
    }
  }
  std::cout << "\t---Done.\n";
  std::cout << "Hash table created and saved (\"" << output_file_ << "\").\n";
  ShowInfo(num_of_empty_buckets, highest_num_of_items_in_a_bucket);
  std::cout << "Program terminated.";
}

HashTableReader::HashTableReader(const std::string &hash_table_file)
    : hash_table_file_(hash_table_file),
      hash_table_values_(GetHashTableValues()) {
  std::cout << "Your hash table file contains\n\t" << hash_table_values_[1] << " word vectors\n\twith " << hash_table_values_[0] << " dimensions in " << hash_table_values_[2] << " buckets.\n";
}

HashTableReader::~HashTableReader() {}

std::vector<int> HashTableReader::GetHashTableValues() {
// Reads the first line of the hash table file that contains the most important
// values of the hash table - they will be returned in a std::vector<int> with
// the value corresponding to the index 0 being the vector size (i.e. the
// number of dimensions of the vectors), the value corresponding to the index 1
// being the number of vectors in the hash table, and the value corresponding
// to the index 2 being the number of buckets the hash table has.
  std::ifstream input_file_stream(hash_table_file_);
  std::string first_line, value;
  std::getline(input_file_stream, first_line);
  std::stringstream stream(first_line);
  std::vector<int> hash_table_values(3);
  for (unsigned i = 0; i < hash_table_values.size(); ++i) {
    getline(stream, value, ',');
    hash_table_values[i] = std::stoi(value);
  }
  return hash_table_values;
}

void HashTableReader::CompareWordVectors(const std::vector<std::string> &words) {
// Creates a "HashTable" and starts the comparison of the "words".
  HashTable HT(hash_table_values_[2]);
  GetVectors(HT, words);
}

void HashTableReader::GetVectors(HashTable &hash_table, const std::vector<std::string> &words) {
// Collects the "vectors" corresponding to both "words" and passes them to
// "ShowSimilarity()" (if both words were found in the hash table file).
  std::vector<std::vector<double>> vectors(2, std::vector<double>(hash_table_values_[0], 0)); // every value of both vectors is 0 by default in order to evaluate later if the "words" were found (if not all values stay 0)
  const std::vector<int> indices = {hash_table.GetIndex(words[0]), hash_table.GetIndex(words[1])};
  std::string line, word_vector, current_index;
  std::vector<std::string> word_vectors_of_line;
  // "loop_forward" helps to make sure (within the for-loop) that the first of
  // the buckets of interest will be read first (i.e. the bucket with the
  // smaller index). Therefore, the file will be read only once from its
  // beginning to its end in the worst case (in order to check if both "words"
  // are contained in the hash table file and - if so - to return their vectors).
  const bool loop_forward = (indices[0] < indices[1])? true : false;
  std::ifstream hash_table_file_stream(hash_table_file_);
  std::getline(hash_table_file_stream, line); // skips first line of the hash table file, which should contain no vectors
  for (int i = (loop_forward? 0 : 1); (loop_forward? i < 2 : i > -1); (loop_forward? ++i : --i)) {
    current_index = std::to_string(indices[i]);
    while (std::getline(hash_table_file_stream, line)) {
      if (CheckIndex(current_index, line)) {
        if (indices[0] == indices[1]) { // if both words are supposed to be in the same bucket
          GetBothVectors(line, words, vectors);
          if (!VectorIsValid(words[0], vectors[0]) || !VectorIsValid(words[1], vectors[1])) // if at least one of the words couldn't be found in the hash table file
            return;
          i = loop_forward? 2 : -1; // makes sure that "break" (in the next line) affects both loops
          break;
        }
        word_vector = GetWordVectorsFromLine(line, words[i]);
        if (word_vector != "") // if the word was found in the hash table file
          vectors[i] = GetVector(word_vector);
        else { // if at least one of the words (i.e. "words[i]") couldn't be found in the hash table file
          std::cout << "\t\"" << words[i] << "\" couldn't be found in your data! Comparison impossible.\n\n";
          return;
        }
        break; // if both "words" got different keys ("indices") and the first one was found
      }
    }
  }
  hash_table.ShowSimilarity(words, vectors);
}

void HashTableReader::GetBothVectors(const std::string &line, const std::vector<std::string> &words_to_find, std::vector<std::vector<double>> &vectors) {
// Searches for the vectors corresponding to both "words_to_find" and
// overwrites the "vectors" (which contain only zeros by default) with the
// values found in the line of the hash table file.
  std::stringstream stream_of_line(line);
  std::vector<std::string> word_vectors_of_line;
  std::string word_vector;
  int i = 0, j = 2;
  while (getline(stream_of_line, word_vector, ',')) {
    for (int k = i; k < j; ++k) {
      if (word_vector.substr(0, words_to_find[k].length()) == words_to_find[k]) {
        vectors[k] = GetVector(word_vector);
        (k == 0)? i = 1 : j = 1; // makes sure that the next "word_vector"s will only be checked for the "word_to_find" that was not found yet
        break;
      }
    }
    if (i == 1 && j == 1) // if both "words_to_find" were found
      break;
  }
}

std::string HashTableReader::GetWordVectorsFromLine(const std::string &line, const std::string &word_to_find) {
// Searches for the word vector of the "word_to_find" in the "line" and returns
// it as a string (if the "word_to_find" was found). If the "word_to_find"
// couldn't be found an empty string will be returned.
  std::stringstream stream_of_line(line);
  std::vector<std::string> word_vectors_of_line;
  std::string word_vector;
  while (getline(stream_of_line, word_vector, ',')) {
    if (word_vector.substr(0, word_to_find.length()) == word_to_find)
      return word_vector;
  }
  return "";
}

std::vector<double> HashTableReader::GetVector(const std::string &word_vector) {
// Takes the string "word_vector" and returns the actual vector as a
// std::vector<double>.
  std::vector<double> vector(hash_table_values_[0]);
  std::stringstream stream(word_vector);
  std::string value;
  getline(stream, value, ' '); // skips the first value, which is the "word" of the "word_vector"
  for (unsigned i = 0; i < vector.size(); ++i) {
    getline(stream, value, ' ');
    vector[i] = atof(value.c_str());
  }
  return vector;
}
