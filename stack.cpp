#include "stack.h"

struct Stack StackNew_(const char* name, const char* func, const char* file, size_t line) {
    struct StackInfo info = { NULL, NULL, NULL, 0 };
    struct Stack stk = { NAN, NULL, 0, 0, info, 0, 0, NAN };

    stk.capacity = START_CAPACITY;
    stk.data = (Elem_t*)calloc(stk.capacity, sizeof(char)); 
    stk.data[0] = POISON;
    stk.Size = 0;

    info.name = name;
    info.func = func;
    info.file = file;
    info.line = line;

    stk.info = info;

    stk.canary0 = rand() ^ (size_t) stk.data ^ 0xDEADF00D;
    stk.canary1 = stk.canary0;

    StackHash(&stk);

    StackDump(&stk);

    return stk;
}

int StackPush(struct Stack *stk, Elem_t value) {
    ASSERT_OK(stk);

    if(stk->Size >= stk->capacity)
        StackResize(stk, 2 * stk->capacity);
    
    stk->data[stk->Size++] = value;

    RETURN StackError(stk);
}

Elem_t StackPop(struct Stack *stk, int* err) {
    ASSERT_OK(stk);

    Elem_t res = stk->data[stk->Size];

    stk->data[stk->Size--] = POISON;

    StackHash(stk);

    if(stk->Size + 1 <= stk->capacity / 2 && stk->Size > 0)
        StackResize(stk, stk->capacity / 2);

    *err = StackError(stk);

    RETURN res;
}

int StackResize(struct Stack *stk, size_t newCapacity) {
    ASSERT_OK(stk);

    stk->data = (Elem_t*)recalloc(stk->data, newCapacity, sizeof(Elem_t));

    if(stk->capacity < newCapacity)
        for(size_t i = stk->capacity; i < newCapacity; i++)
            stk->data[i] = POISON;

    stk->capacity = newCapacity;

    RETURN StackError(stk);
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

void StackDump_(struct Stack *stk, const char *func, const char *file, size_t line) {
    struct StackInfo* info = NULL;
    FILE *fp = NULL;

    info = &(stk->info);
    fp = fopen(LOGPATH, "a");

    fprintf(fp, "%s at %s(%u)\n", func, file, line);
    fprintf(fp, "Stack[%lx] (ok) at %s at %s(%u) {\n", (long) stk, info->func, info->file, info->line);
    fprintf(fp, "\tSize = %u\n", stk->Size);
    fprintf(fp, "\tcapacity = %u\n", stk->capacity);
    fprintf(fp, "\tdata[%lx] {\n", (long) info->name);
    
    for(size_t i = 0; i < stk->capacity; i++) {
        fprintf(fp, "\t\t");

        if(i <= stk->Size)
            fprintf(fp, "*");
        
        fprintf(fp, "[%u] = %c", i, stk->data[i]);

        if(stk->data[i] == POISON)
            fprintf(fp, " (POISON)");
        
        fprintf(fp, "\n");
    }

    fprintf(fp, "\t}\n}\n\n");

    fclose(fp);

    return;
}

int StackError(struct Stack *stk) {
    int err = 0;

    err |= CHECK(stk, NULL_POINTER);

    err |= CHECK(stk->data, NULL_DATA);
    
    err |= CHECK(stk->Size < BIG_UNS, BAD_SIZE);
    
    err |= CHECK(stk->capacity < BIG_UNS && stk->capacity > 0, BAD_CAPACITY);

    err |= CHECK(stk->canary0 == stk->canary1, CORRUPTED_CANARIES);

    err |= CHECK(stk->stackhash == StackHash(stk), CORRUPTED_STACK);

    err |= CHECK(checkdatacanaries(stk), CORRUPTED_DATA_CANARIES);

    //err |= CHECK(stk->datahash == hash(data, capacity), CORRUPTED_DATA);
    err |= CHECK(1, CORRUPTED_DATA);
    
    return err;
}

void perror_(int err, const char* file, const char* func, size_t line) {
    FILE* fp = fopen(LOGPATH, "a");

    fprintf(fp, "\n%s in %s at %s(%d)\n\n", binary(err), func, file, line);

    fclose(fp);

    return;
}

void* recalloc(void* ptr, size_t num, size_t Size) {
    assert(ptr && num && Size);

    ptr = realloc(ptr, num * Size);

    return ptr;
}

void CleanLogs() {
    fclose(fopen(LOGPATH, "w"));
}

char* binary(int n) {
    const char *zero = "0b0000000";
    char *res = NULL;
    int i = 0,
        l = strlen(zero);

    res = (char*)calloc(l + 1, sizeof(char));
    strcpy(res, zero);

    while(n > 0) {
        res[l - i] = (char) (n % 2 + '0');
        n /= 2;
        i++;
    }

    return res;
}

long StackHash(struct Stack *stk){
    stk->stackhash = stk->stackhash;
    stk->stackhash = 0;

    stk->stackhash = hash(stk, sizeof(*stk));

    return stk->stackhash;
}

long hash(void* p, size_t size) {
    long res = 0;
    char* ptr = NULL;

    ptr = (char*)p;

    for(size_t i = 0; i < size; i++) {
        res = ((res << 5) + res) + ptr[i];
    }

    return res;
}

int checkdatacanaries(struct Stack *stk) {
    return 1;
}

//TODO: snippet FILE* fp = NULL;