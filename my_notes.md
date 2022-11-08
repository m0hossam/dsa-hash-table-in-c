# Notes For Future Self

### Pointer Hell
- Only memory that's allocated using malloc() needs to be freed
- You can't free a struct before freeing its dynamically allocated members
- Some functions uses malloc() internally like strdup()
- Set pointers to NULL after freeing, in case of accidental usage after freeing
- Avoid dangling pointers, if you free a pointer the consider all pointers that pointed to it
- For contiguous allocation, use calloc()
- Use xmalloc() and xcalloc() for program safety

### Quirks Of C
- Self-referential structures should be named differently in the typedef to allow self-reference
- Using `typedef struct` instead of `struct` omits the need to type `struct` before variable names
- `const char*` is a **mutable** pointer to an **immutable** string/char
- [size_t](https://stackoverflow.com/questions/2550774/what-is-size-t-in-c) is used to represent the size of an object
