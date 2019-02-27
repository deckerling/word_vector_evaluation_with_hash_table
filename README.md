# *word_vector_evaluation_with_hash_table*
It can be annoying to calculate similarities of word vectors with programs using simple algorithms to find the vectors in question in a big file, especially if you want to compare many word vectors in a row. For this reason *word_vector_evaluation_with_hash_table* stores all the vectors of a "word vector file" in a hash table in order to provide fast access to them. Done that, the program allows you to repeatedly calculate the Euclidean distance and the cosine similarity of two word vectors.

## Modes and requirements
*word_vector_evaluation_with_hash_table* provides three modes:
1. "Hash Table on Memory":  
A mode to create a hash table on memory storing the word vectors from a word vector file in order to calculate similarities between the vectors.
2. "Hash Table File Writer":  
A mode to create a hash table storing the vectors from a word vector file; the hash table will be written to a file and therefore being "saved".
3. "Hash Table File Reader":   
A mode to read a hash table file (created by the second mode) in order to calculate similarities between the vectors.

The first mode requires a word vector file as argument. The file should contain one word vector per line with all elements of a line being separated by a whitespace and the first element being the word and all others being the values of the vector. Of course, all word vectors should have the same number of dimensions (i.e. the same "vector size"). **If the word vector file is structured in a different way, errors may occur.** Suitable word vector files can be for example created with [Stanford's GloVe implementation](https://github.com/stanfordnlp/GloVe).  
The second mode requires such a word vector file as well as a first argument, but also a second argument being the name of an output file the hash table should be written to (this file should not exist before or should at least be empty).  
The third mode requires a hash table file as argument (i.e. an output file of the second mode).

The measurements for word vector similarities that can be calculated with the first or third mode are both the cosine similarity of two vectors and the Euclidean distance between them.

## License
The work contained in this package is licensed under the Apache License, Version 2.0 (see the file "[LICENSE](LICENSE)").
