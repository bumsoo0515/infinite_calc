#include "fraction.c"

typedef struct token {
    bool type;
    fraction* num;
    char op;
} token;

typedef struct stnode {
    struct stnode *next;
    token *data;
} stnode;