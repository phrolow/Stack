#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//#define ASSERTED == 0 || Error("Error")
#define RETURN StackDump(stk)   \
                                \
        return;

#define ASSERT_OK(stk)      if(StackError(stk))    {            \
                                abort();                        \
                            }                                   \
                                                                \
                            StackDump(stk);

typedef char Elem_t;

const Elem_t POISON = 7;
const size_t START_CAPACITY = 1;
const char *LOGFILE = "log.txt";

struct Stack {
    Elem_t* data;
    size_t Size;
    size_t capacity;
    char* name;
    char* func;
    char* file;
    char* line;
};

struct Stack StackNew_(char* name, char* func, char* file, size_t line);

int StackPush(struct Stack *stk, Elem_t value);

Elem_t StackPop(struct Stack *stk);

void StackResize(struct Stack *stk, size_t newCapacity);

void StackDtor(struct Stack *stk);

void StackDump_(struct Stack *stk, char* func, char* file, size_t line);

int StackError(struct Stack *stk);

void* recalloc(void* ptr, size_t num, size_t Size);

#endif // STACK_H_INCLUDED
