#include "calc.c"

#define PRINT_LIMIT 1000

int main() {
    Stack infix; infix.top = NULL;
    Stack postfix; postfix.top = NULL;

    // FILE *fp = fopen("input.txt","r");
    FILE *fp = stdin;
    if (fp == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    printf("\n---- Infinite Calculator ----\n");
    printf("Input:\n");
    node *inp = new_list();
    while (1) {
        char c = getc(fp);
        if (c == EOF) break;
        putchar(c);
        push_back(inp, c);
    }
    fclose(fp);

    list_tokenize(inp, &infix);
    infix_to_postfix(&infix, &postfix);

    fraction *res = calculate(&postfix);
    
    printf("\n\nOutput:\n");
    print_fraction(res, PRINT_LIMIT);
    printf("\n");

    free_fraction(res);
    free_list(inp);

    return 0;
}