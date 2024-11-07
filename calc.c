#include "postfix.c"

fraction* calculate(Stack *p) {
    Stack s; s.top = NULL;

    while (!stempty(p)) {
        if (top(p)->dtype == 0) {
            push_num(&s, top(p)->num); pop(p);
            continue;
        }
        fraction *x = top(&s)->num; pop(&s);
        fraction *y = top(&s)->num; pop(&s);

        fraction *r = NULL;
        char op = top(p)->op; pop(p);
        if (op=='+') r = FADD(x, y);
        else if (op=='-') r = FSUB(x, y);
        else if (op=='*') r = FMUL(x, y);
        else if (op=='/') r = FDIV(x, y);
        free_fraction(x);
        free_fraction(y);
        push_num(&s, r);
    }

    return top(&s)->num;
}