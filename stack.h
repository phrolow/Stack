#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ASSERTED == 0 || Error("Error")

#define ASSERT_OK(stk)                                                              \
    StackHash(stk);                                                                 \
                                                                                    \
    if(StackError(stk)) {                                                           \
        perror_(StackError(stk), LOCATION);                                         \
        abort();                                                                    \
    }                                                                               \
                                                                                    \
    StackDump(stk)

#define CHECK(cond, err) (cond) ? 0 : (err)

#define RETURN                                                                      \
        StackHash(stk);                                                             \
        stk->stackhash1 = stk->stackhash0;                                          \
                                                                                    \
        ASSERT_OK(stk);                                                             \
                                                                                    \
        return

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__

#define StackDump(stk) StackDump_(stk, LOCATION)

#define StackNew(stk) StackNew_(#stk + (#stk[0] == '&'), LOCATION)

typedef char Elem_t;

struct StackInfo {
    const char* name;
    const char* func;
    const char* file;
    size_t line;
};

struct Stack {
    long double canary0;
    Elem_t* data;
    size_t Size;
    size_t capacity;
    struct StackInfo info;
    long datahash0;
    long datahash1;
    long stackhash0;
    long stackhash1;
    long double canary1;
};

enum Error {
    NULL_POINTER = 0b1,
    NULL_DATA = 0b10,
    BAD_SIZE = 0b100,
    BAD_CAPACITY = 0b1000,
    CORRUPTED_CANARIES = 0b10000,
    CORRUPTED_STACK = 0b100000,
    CORRUPTED_DATA_CANARIES = 0b1000000,
    CORRUPTED_DATA = 0b10000000
};

const Elem_t POISON = 7;
const size_t BIG_UNS = 1000000;
const size_t START_CAPACITY = 1;
const char* const ElemFormat = "%d";
const char* const LOGPATH = "log.txt";

struct Stack StackNew_(const char* name, const char* func, const char* file, size_t line);

int StackPush(struct Stack *stk, Elem_t value);

Elem_t StackPop(struct Stack *stk, int *err);

int StackResize(struct Stack *stk, size_t newCapacity);

void StackDtor(struct Stack *stk);

void StackDump_(struct Stack *stk, const char* func, const char* file, size_t line);

int StackError(struct Stack *stk);

void StackHash(struct Stack *stk);

void perror_(int err, const char* file, const char* func, size_t line);

void* recalloc(void* ptr, size_t num, size_t Size);

void CleanLogs();

char* binary(int n);

long hash(void* ptr, size_t size);

int checkdatacanaries(struct Stack *stk);

#endif // STACK_H_INCLUDED
