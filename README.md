# Hash Table Implementation In C

This is a simple implementation of the hash-table data structure in C, using linked lists & separate chaining for collision handling. I created this project to practice my humble knowledge of data structures & algorithms and the C programming language. Through this project, I tackled challenges involving insertion, deletion & search operations in both a singly linked list and a hash table. I also learned quite a bit about memory management and how to use pointers safely.

## Project Specifications
- The hash table stores string only key-value pairs
- The hash table has an initial size of 53 pairs
- The hash table can be resized to bigger & smaller sizes according to its load
- Collision handling is done using the separate chaining technique
- The separate chaining technique is implemented using a singly linked list

## The Mechanism Behind A Hash Table
A hash table maps unique keys to values like a dictionary, it uses a hash algorithm to convert the key string into an index where the pair gets stored.
### Operations
- **Insertion**: The key gets hashed into an index by a hashing function, then the pair is stored in that index.
    - *Time Complexity: O(1) in best & average cases*
- **Search**: The key gets hashed into an index by a hashing function, then the value mapped to it is returned.
    - *Time Complexity: O(1) in best & average cases*
- **Deletion**: The key gets hashed into an index by a hashing function, then the pair is deleted.
    - *Time Complexity: O(1) in best & average cases*
### Collision Handling 
Hashing functions are not perfect, some keys will get hashed into the same index, to handle this we use one of 2 methods:
- **Separate Chaining**: Adding the collided key to a linked list stored in the same index.
    - *ex. Singly Linked Lists used in this project*
- **Open Addressing**: Finding another index for the collided key using predetermined algorithms.
    - *ex. Double Hashing*

## Credits
- James Routley's [write-a-hash-table](https://github.com/jamesroutley/write-a-hash-table) tutorial
  - This tutorial explained the underlying mechanism of hash-tables & double-hashing clearly, a significant part of my code is similar to the tutorial's code.
- Stack Overflow & the internet in general
  - Various late nights spent googling errors can attest.
- Some Books
  - Data Structures & Algorithms in Java by Robert Lafore helped me with the linked list, while The C Programming Language helped me with pointers.
