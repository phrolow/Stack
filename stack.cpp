#include "stack.h"

struct Stack StackNew_(const char* name, const char* func, const char* file, size_t line) {
    struct StackInfo info = { NULL, NULL, NULL, 0 };
    struct Stack stk = { NAN, NULL, 0, 0, info, 0, 0, NAN };
    long double datacanary = NAN;

    stk.capacity = START_CAPACITY;
    stk.data = (Elem_t*)calloc(ON_CANARY_PROT(2 * sizeof(datacanary) / sizeof(Elem_t)) + stk.capacity, sizeof(char)); 
    stk.Size = 0;

    info.name = name;
    info.func = func;
    info.file = file;
    info.line = line;

    stk.info = info;

    #ifdef CANARY_PROT
    stk.canary0 = rand() ^ (size_t) stk.data ^ 0xDEADF00D;
    stk.canary1 = stk.canary0;

    datacanary = rand() ^ ((size_t) stk.data + 1) ^ 0xDEADF00D;

    *((long double *) (stk.data)) = datacanary;

    stk.data += sizeof(datacanary) / sizeof(Elem_t);

    *((long double *) (stk.data + stk.capacity)) = datacanary;

    #endif

    stk.data[0] = POISON;

    StackDataHash(&stk);
    StackHash(&stk);

    StackDump(&stk);

    return stk;
}

void StackPush(struct Stack *stk, Elem_t value) {
    ASSERT_OK(stk);

    if(stk->Size >= stk->capacity)
        StackResize(stk, 2 * stk->capacity);
    
    stk->data[stk->Size++] = value;

    RETURN;
}

Elem_t StackPop(struct Stack *stk) {
    ASSERT_OK(stk);

    Elem_t res = stk->data[--stk->Size];

    stk->data[stk->Size] = POISON;

    StackDataHash(stk);
    StackHash(stk);

    if(stk->Size + 1 <= stk->capacity / 2 && stk->Size > 0)
        StackResize(stk, stk->capacity / 2);

    RETURN res;
}

void StackResize(struct Stack *stk, size_t newCapacity) {
    ASSERT_OK(stk);

    stk->data = (Elem_t*)recalloc(stk->data ON_CANARY_PROT(- sizeof(long double) / sizeof(Elem_t)), newCapacity ON_CANARY_PROT(+ 2 * sizeof(long double) / sizeof(Elem_t)), sizeof(Elem_t));
    stk->data += sizeof(long double) / sizeof(Elem_t);
    
    for(size_t i = 0; i < sizeof(long double) / sizeof(Elem_t); i++) {
        *(stk->data + stk->capacity + i) = POISON;
    }
    
    *((long double *) (stk->data + newCapacity)) = *((long double *) (stk->data - sizeof(long double)));

    if(stk->capacity < newCapacity)
        for(size_t i = stk->capacity; i < newCapacity; i++)
            stk->data[i] = POISON;

    stk->capacity = newCapacity;

    RETURN;
}

void StackDtor(struct Stack *stk) {
    ASSERT_OK(stk);

    for(size_t i = 0; i <= stk->capacity; i++)
        stk->data[i] = POISON;
    
    stk->capacity = 0;
    stk->Size = 0;

    free(stk->data);

    StackDump(stk); 

    return;
}

int checkdatacanaries(struct Stack *stk) {
    return *((long double *) (stk->data - sizeof(long double) / sizeof(Elem_t))) == *((long double *) (stk->data + stk->capacity));
}
