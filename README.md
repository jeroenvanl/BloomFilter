# BloomFilter
This code is an extension to the Hashing assignment for the "COMSM1201: Programming in C" course at the University of Bristol (UK). You can find a short description of the algorithm below.

An interesting antidote to hashing is the use of a Bloom filter,
a probalistic data structure which does not store the actual keys,
and consequently is much more efficient with memory.

It does so by creating a large array of bits, which is loaded with 
(char) '1' if a hash function for a key returns that position. When you 
do this for multiple hash functions (but not too many), then you create 
a vector of positions in which the key is stored. When you then want to 
check whether a key is in the dictionary, you simply check whether all 
of the bits from the hash functions are turned on. Then you know (with 
a small margin for error) whether that the word is in the dictionary. This 
is useful when you want to check a long key (for instance a long string)
against a large set of data, such as a dictionary.

The trade-off is that you can never be 100% sure whether a word is
in the dictionary, you can only be sure when it is not. In this example,
I set the false-positive rate to be .01, which means I want to be 99% sure
a word is in the dictionary.

The Bloom Filter is inspired by this paper: Kirsch, Adam, and Michael Mitzenmacher.
"Less hashing, same performance: Building a better Bloom filter."
European Symposium on Algorithms. Springer Berlin Heidelberg, 2006.

