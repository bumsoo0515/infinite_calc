#include "tokenize.c"

int precedence(token *a) {
    assert(a->dtype == OPERATOR);
    if (a->op=='+' || a->op=='-') return 1;
    if (a->op=='*' || a->op=='/') return 2;
    if (a->op=='@') return 3;
    return 0;
}

void infix_to_postfix(Stack *infix, Stack *postfix) {
    Stack oper; oper.top = NULL;
    Stack tmp; tmp.top = NULL;

    while (!stempty(infix)) {
        token *v = top(infix);
        if (v->dtype == NUMBER) {
            push_num(&tmp, v->num);
        }
        else if (v->op=='(') {
                push_op(&oper, v->op);
        }
        else if (v->op==')') {
            while (!stempty(&oper) && top(&oper)->op!='(') {
                push_op(&tmp, top(&oper)->op);
                pop(&oper);
            }
            pop(&oper);
        }
        else {
            while (!stempty(&oper) && precedence(top(&oper)) >= precedence(v)) {
                push_op(&tmp, top(&oper)->op);
                pop(&oper);
            }
            push_op(&oper, v->op);
        } 
        pop(infix);
    }

    while (!stempty(&oper)) {
        push_op(&tmp, top(&oper)->op);
        pop(&oper);
    }

    // tmp에 역순으로 담긴 후위식을 postfix에 다시 담기
    while (!stempty(&tmp)) {
        if (top(&tmp)->dtype == NUMBER) push_num(postfix, top(&tmp)->num);
        else push_op(postfix, top(&tmp)->op);
        pop(&tmp);
    }
}

/*
// TESTCODE
int main() {
    char str[] = "-23456789876.5432*5765752765127.357+(-2131231.0-2121241)/2+0";
    Stack infix; infix.top = NULL;
    Stack postfix; postfix.top = NULL;

    tokenize(str, &infix);
    infix_to_postfix(&infix, &postfix);

    while (!stempty(&postfix)) {
        if (top(&postfix)->dtype == NUMBER) print_fraction(top(&postfix)->num, 500);
        else printf("%c", top(&postfix)->op);
        printf("\n");
        fflush(stdout);
        pop(&postfix);
    }
    return 0;
}
// */