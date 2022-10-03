#include "stack.h"

int main() {
    double a = -2.07742e+14;
    double b = a;
    printf("Main: %lg %lg\n", a, b);

    CleanLogs();
    
    struct Stack stk1 = StackNew();
    Elem_t value = 7;

    StackPush(&stk1, 'w');
    StackPush(&stk1, 'a');
    StackPush(&stk1, 's');
    StackPush(&stk1, 'w');

    value = StackPop(&stk1);
    value = StackPop(&stk1);
    value = StackPop(&stk1);

    printf(ElemFormat, value);

    StackDtor(&stk1);

    return 0;
}