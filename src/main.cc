// main.cc

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

#include <fstream>
#include <regex>

#include "wvewht.h" // wvewht = "word_vector_evaluation_with_hash_table"

std::string SetToLowerCase(std::string string) {
// Sets every character of a string to lower case and returns the string as a
// whole.
  for (auto& character : string)
    character = tolower(character);
  return string;
}

template <typename T>
void StartComparing(T& hash_table) {
// Allows the user to enter two words to compare using the word vectors.
  std::vector<std::string> words(2);
  while (true) {
    for (int i = 0; i < 2; ++i) {
      std::cout << "Enter a word you want to compare to " << ((i == 0)? "another" : "\""+words[0]+"\"") << " (enter 'x' to terminate the program):\n";
      std::cin >> words[i];
      // The following line does only make sense if all of the "words" in your
      // word vector file are in lower case - if not, you should delete it.
      words[i] = SetToLowerCase(words[i]);
      if (words[i] == "x")
        return;
    }
    hash_table.CompareWordVectors(words);
  }
}

bool IsInteger(const std::string& string_to_check) {
// Returns "true" if "string_to_check" equals an integer and "false" otherwise.
  for (auto& character : string_to_check) {
    if (std::isdigit(character) == 0)
      return false;
  }
  return true;
}

bool IsHashTableFile(const std::string& file_to_check) {
// Checks if the given file is a hash table file or a "normal" word vector file
// by reading the first line. If the file is a hash table file created by this
// program, the first line contains only three integers separated by ',' - if
// that is the case "true" will be returned and "false" otherwise.
  std::ifstream file_stream(file_to_check);
  std::string first_line, value;
  std::getline(file_stream, first_line);
  std::stringstream stream(first_line);
  std::vector<std::string> check_vector;
  int vector_size_count = 0;
  while (std::getline(stream, value, ',')) {
    if (vector_size_count == 3)
      return false;
    check_vector.push_back(value);
    vector_size_count++;
  }
  if (vector_size_count != 3)
    return false;
  for (auto& value : check_vector) {
    if (!IsInteger(value))
      return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
// At least one additional argument is needed.
// Case 1: If you want to create a hash table only on memory, a word vector
//  file should be given.
// Case 2: If you want to work on a hash table created by this program and
//  saved in a file, that hash table file has to be the additional argument.
// Case 3: If you want to save a hash table containing your word vectors in a
//  file, both a word vector file and an output file are needed as arguments.
  if (argc == 2) { // if one file is given as argument
    if (IsHashTableFile(argv[1])) { // checks if the given file is a hash table file or a "normal" word vector file
      HashTableReader hash_table_reader(argv[1]);
      StartComparing(hash_table_reader);
    } else {
      HashTableOnMemory hash_table_on_memory(argv[1]);
      if (!hash_table_on_memory.HashTableIsValid())
        return -1;
      std::string answer;
      std::cout << "Enter \"prinfo\" to show information about the hash table (number of buckets, number of word vectors in the buckets and load factor); enter anything else to skip:\n";
      std::cin >> answer;
      if (std::regex_match(SetToLowerCase(answer), (std::regex) "prinfo|((print|show)_?info)"))
        hash_table_on_memory.PrintInfo();
      StartComparing(hash_table_on_memory);
    }
    std::cout << "\nProgram terminated.";
    return 0;
  } else if (argc == 3) { // if two files are given as arguments: a hash table will be created and saved in a file
    HashTableWriter HTW(argv[1], argv[2]);
    return 0;
  } else if (argc == 1)
    std::cout << "ERROR: MISSING ARGUMENT - No input file given!\n";
  else
    std::cout << "ERROR: TOO MANY ARGUMENTS - Only one input file needed!\n";
  std::cout << "Style of usage:\n\t.\\wvewht [input_file_containing_word_vectors] [output_file (optional; if this argument is given, the hash table will be written to that \"output_file\")]\n";
  std::cout << "Example usage:\n\t.\\wvewht my_word_vectors.txt my_word_vector_hash_table.csv\n";
  std::cout << "\nProgram terminated.";
  return -1;
}
