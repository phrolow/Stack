#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ASSERTED == 0 || Error("Error")

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__

#define StackDump(stk) StackDump_(stk, LOCATION)

#define StackNew(stk) StackNew_(#stk + (#stk[0] == '&'), LOCATION)

#define RETURN                                                                      \
        ASSERT_OK(stk);                                                             \
                                                                                    \
        StackDump(stk);                                                             \
                                                                                    \
        return

#define ASSERT_OK(stk)                                                              \
    if(StackError(stk)) {                                                           \
        perror_(StackError(stk), LOCATION);                                         \
        abort();                                                                    \
    }                                                                               \
                                                                                    \
    StackDump(stk)

#define CHECK(cond, err) (cond) ? 0 : (err)

typedef char Elem_t;

const Elem_t POISON = 7;
const size_t BIG_UNS = 1000000;
const size_t START_CAPACITY = 1;
const char* const ElemFormat = "%d";
const char* const LOGPATH = "log.txt";

struct StackInfo {
    const char* name;
    const char* func;
    const char* file;
    size_t line;
};

struct Stack {
    Elem_t* data;
    size_t Size;
    size_t capacity;

    struct StackInfo info;
};

enum Error {
    NULL_POINTER = 0b1,
    NULL_DATA = 0b10,
    BAD_SIZE = 0b100,
    BAD_CAPACITY = 0b1000
};

struct Stack StackNew_(const char* name, const char* func, const char* file, size_t line);

int StackPush(struct Stack *stk, Elem_t value);

Elem_t StackPop(struct Stack *stk, int *err);

int StackResize(struct Stack *stk, size_t newCapacity);

void StackDtor(struct Stack *stk);

void StackDump_(struct Stack *stk, const char* func, const char* file, size_t line);

int StackError(struct Stack *stk);

void perror_(int err, const char* file, const char* func, size_t line);

void* recalloc(void* ptr, size_t num, size_t Size);

void CleanLogs();

char* binary(int n);

#endif // STACK_H_INCLUDED
