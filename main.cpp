#include "stack.h"

int main() {
    /* struct Stack st = {LOCATION};
    StackInit(&st, ...);


    VAR(stack, st);
    StackInit(&st, 5);*/

    CleanLogs();
    
    struct Stack stk1 = StackNew();
    int err = 0;
    Elem_t value = 7;

    StackPush(&stk1, 'w');
    StackPush(&stk1, 'a');
    StackPush(&stk1, 's');
    StackPush(&stk1, 'w');

    value = StackPop(&stk1, &err);
    value = StackPop(&stk1, &err);
    value = StackPop(&stk1, &err);

    printf("%c\n", value);

    StackDtor(&stk1);

    return 0;
}