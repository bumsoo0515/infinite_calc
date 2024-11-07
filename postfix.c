#include "tokenize.c"

void infix_to_postfix(Stack *infix, Stack *postfix) {
    while (!stempty(infix)) {
        token *v = infix->top;
        if (v->dtype == NUMBER) {
            push_num(postfix, v->num);
            pop(infix);
        }
        else {
            switch (v->op) {
                case '(':
                    break;
                case ')':
                    break;
                case '+':
                case '-':
                    break;
                case '*':
                case '/':
                    break;
                default:
                    break;
            }
        }
    }
}