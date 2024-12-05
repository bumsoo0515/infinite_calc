#include "postfix.c"

fraction* calculate(Stack *p) {
    Stack s; s.top = NULL;

    while (!stempty(p)) {
        if (is_num(peek(p))) {
            push_num(&s, pop_num(p));
            continue;
        }

        if (stempty(&s)) OperandMissing();
        fraction *y = pop_num(&s);
        
        if (stempty(&s)) OperandMissing();
        fraction *x = pop_num(&s);

        fraction *r = NULL;
        char op = pop_op(p);
        if (op=='+') r = FADD(x, y);
        else if (op=='-') r = FSUB(x, y);
        else if (op=='*' || op=='@') r = FMUL(x, y);
        else if (op=='/') r = FDIV(x, y);
        free_fraction(x);
        free_fraction(y);
        push_num(&s, r);
    }

    return pop_num(&s);
}