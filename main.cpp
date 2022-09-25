#include "stack.h"

int main() {
    struct Stack stk1 = {};
    int err = 0;
    Elem_t value = 7;

    stk1 = StackNew();
    StackPush(&stk1, 'w');
    value = StackPop(&stk1, &err);

    printf("%c", value);

    StackDtor(&stk1);

    return 0;
}
