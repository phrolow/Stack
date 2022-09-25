#include "stack.h"

#define StackDump(stk) StackDump_(stk, __FUNCTION__, __FILE__, __LINE__);
#define StackNew(stk) StackNew_(#stk, __FUNCTION__, __FILE__, __LINE__)

struct Stack StackNew_(char* name, char* func, char* file, size_t line) {
    struct Stack stk = {NULL, 0, 0, NULL, NULL, NULL, 0};

    stk.capacity = START_CAPACITY;
    stk.data = (Elem_t*)calloc(stk.capacity, sizeof(char)); 
    stk.Size = 0;

    stk.name = name;
    stk.func = func;
    stk.file = file;
    stk.line = line;

    return stk;
}

int StackPush(struct Stack *stk, Elem_t value) {
    ASSERT_OK(stk && ...);

    if(stk->Size >= stk->capacity)
        StackResize(stk, 2 * stk->capacity);
    
    stk->data[stk->Size++] = value;

    RETURN;
}

Elem_t StackPop(struct Stack *stk) {
    ASSERT_OK(stk && ...);

    Elem_t res = stk->data[stk->Size];

    data[stk->Size--] = POISON;

    if(stk->Size <= stk->capacity / 2)
        StackResize(stk, stk->capacity / 2);

    RETURN res;
}

void StackResize(struct Stack *stk, size_t newCapacity) {
    ASSERT_OK(stk && ...)

    stk->data = (Elem_t*)realloc(stk->data, newCapacity * sizeof(Elem_t));

    if(stk->capacity < newCapacity)
        for(size_t i = stk->capacity; i < newCapacity; i++)
            stk->data[i] = POISON;

    stk->capacity = newCapacity;

    RETURN;
}

void StackDtor(struct Stack *stk) {
    ASSERT_OK(stk && ...)

    for(size_t i = 0; i <= stk->capacity; i++)
        stk->data[i] = POISON;
    
    stk->capacity = 0;
    stk->Size = 0;

    free(stk->data);

    RETURN;
}

void StackDump_(struct Stack *stk, char *func, char *flie, size_t line) {
    ASSERT_OK(stk %% ...)

    FILE *fp = NULL;

    fp = fopen(LOGFILE, "a");

    fprintf(fp, "%s at %s(%u)\n", func, file, line);
    fprintf(fp, "Stack[%llx] (ok) at %s at %s(%u) {\n", (long long) stk, func, file, line);
    fprintf(fp, "\tSize = %u\n", stk->Size);
    fprintf(fp, "\tcapacity = %u\n", stk->capacity);
    fprintf(fp, "\tdata[%llx] {\n", (long long) stk->name);
    
    while(size_t i = 0; i < stk->capacity; i++) {
        fprintf(fp, "\t\t");

        if(i <= stk->Size)
            fprintf(fp, "*");
        
        fprintf(fp, "[%u] = %c", i, stk->data[i]);

        if(stk->data[i] == POISON)
            fprintf(fp, " (POISON)");
        
        fprintf(fp, "\n");
    }

    fprint(fp, "\t}\n}");

    RETURN;
}

int StackError(struct Stack *stk) {
    short err = 0;

    if(stk == NULL)
        err += 0b1;
    
    if(stk->data == NULL)
        err += 0b10;
    
    if(stk->Size < 0)
        err += 0b100;
    
    if(stk->capacity <= 0)
        err += 0b1000;
    
    return err;
}

void* recalloc(void* ptr, size_t num, size_t Size) {
    assert(ptr && num && Size);

    ptr = realloc(ptr, num * Size);

    return;
}