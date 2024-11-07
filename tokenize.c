#include "fraction.c"

#define NUMBER 0
#define OPERATOR 1

// 식의 각 토큰을 담으면서 동시에 스택의 노드 역할
typedef struct token {
    bool dtype;
    fraction *num;
    char op;
    struct token *next;
} token;

typedef struct Stack {
    token *top;
} Stack;

bool stempty(Stack *s) {
    return s->top == NULL;
}

void pop(Stack *s) {
    token *to_del = s->top;
    s->top = s->top->next;
    free(to_del);
    // *주의* 안에 fraction이 들어 있다면 이는 free되지 않는다
}

token* top(Stack *s) {
    return s->top;
}

void push_op(Stack *s, char op) {
    token *to_add = (token*)malloc(sizeof(token));
    to_add->dtype = OPERATOR;
    to_add->num = NULL;
    to_add->op = op;
    to_add->next = s->top;
    s->top = to_add;
}

void push_num(Stack *s, fraction *f) {
    token *to_add = (token*)malloc(sizeof(token));
    to_add->dtype = NUMBER;
    to_add->num = f;
    to_add->op = 0;
    to_add->next = s->top;
    s->top = to_add;
}

void tokenize(char *str, Stack *res) {
    Stack tmp; tmp.top = NULL;
    // 먼저 앞에서부터 토큰을 분해해 스택에 순서대로 넣고,
    // 그걸 다시 빼서 나오는 순서대로 결과 스택에 다시 넣으면
    // 식의 맨 앞 토큰이 스택의 top에 제대로 오게 된다.

    fraction *cur = NULL;
    bool underpoint = 0;

    for (char *it = str; *it!='\0'; it++) {
        if (*it>='0' && *it<='9') {
            if (cur == NULL) {
                cur = new_fraction();
                cur->numer = new_bigint();
                cur->denom = str_to_bigint("1");
            }
            push_back(cur->numer->head, *it-'0');
            if (underpoint) push_back(cur->denom->head, 0);
        }
        else if (*it=='.') underpoint = 1;
        else if ((*it=='+' || *it=='-') && (cur==NULL)) {
            // +나 -가 수의 부호를 표현하는 경우.
            cur = new_fraction();
            cur->numer = new_bigint();
            cur->denom = str_to_bigint("1");
            cur->numer->sign = (*it=='+' ? PLUS : MINUS);
        }
        else if (*it=='+' || *it=='-' || *it=='*' || *it=='/' || *it=='(' || *it==')'){
            if (cur != NULL) {
                push_num(&tmp, cur);
                cur = NULL;
                underpoint = 0;
            }
            push_op(&tmp, *it);
        }
    }
    if (cur != NULL) push_num(&tmp, cur);
    
    while (!stempty(&tmp)) {
        if (top(&tmp)->dtype == NUMBER) {
            push_num(res, top(&tmp)->num);
        }
        else {
            push_op(res, top(&tmp)->op);
        }
        pop(&tmp);
    }
}

/*
// TESTCODE
int main() {
    char str[] = "-123.3+564-(23*535.33232)/2";
    Stack res; res.top = NULL;

    tokenize(str, &res);
    while (!stempty(&res)) {
        if (top(&res)->dtype == NUMBER) print_fraction(top(&res)->num, 500);
        else printf("%c", top(&res)->op);
        printf("\n");
        fflush(stdout);
        pop(&res);
    }
}
// */