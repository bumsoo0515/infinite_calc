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
        token *v = peek(infix);
        if (v->dtype == NUMBER) {
            push_num(&tmp, v->num);
        }
        else if (v->op=='(') {
            push_op(&oper, v->op);
        }
        else if (v->op==')') {
            while (!stempty(&oper) && peek(&oper)->op!='(') {
                push_op(&tmp, pop_op(&oper));
            }
            pop(&oper); // 여는 괄호 제거
        }
        else {
            while (!stempty(&oper) && precedence(peek(&oper)) >= precedence(v)) {
                push_op(&tmp, pop_op(&oper));
            }
            push_op(&oper, v->op);
        }
        pop(infix);
    }

    while (!stempty(&oper)) push_op(&tmp, pop_op(&oper));

    // tmp에 역순으로 담긴 후위식을 postfix에 다시 담기
    while (!stempty(&tmp)) {
        if (peek(&tmp)->dtype == NUMBER) push_num(postfix, pop_num(&tmp));
        else push_op(postfix, pop_op(&tmp));
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
        if (peek(&postfix)->dtype == NUMBER) print_fraction(peek(&postfix)->num, 500);
        else printf("%c", peek(&postfix)->op);
        printf("\n");
        fflush(stdout);
        pop(&postfix);
    }
    return 0;
}
// */