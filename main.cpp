#include "stack.h"

int main() {
    CleanLogs();

    struct Stack stk1 = StackNew();
    Elem_t value = 7;

    StackPush(&stk1, 'w');
    StackPush(&stk1, 'a');
    StackPush(&stk1, 's');

    stk1.data[1] = 'N';
    stk1.Size = 228;

    StackPush(&stk1, 'w');

    value = StackPop(&stk1);
    value = StackPop(&stk1);
    value = StackPop(&stk1);

    printf(ElemFormat, value);

    StackDtor(&stk1);

    return 0;
}