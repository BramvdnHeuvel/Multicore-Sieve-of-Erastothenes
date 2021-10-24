// big_int.h
// This file defined how long integers should be
// 
// Smaller numbers mean the program runs faster,
// but it allows for lesser processors at once
// as a result of integer overflow.
//
// The numbers should be at least half the integer range.

// Up to 32,767 processors
typedef unsigned int BIG_INT;

// Up to 2,147,483,647 processors
// typedef unsigned long int BIG_INT;

// Up to 9,223,372,036,854,775,807 processors
// typedef unsigned long long int BIG_INT;