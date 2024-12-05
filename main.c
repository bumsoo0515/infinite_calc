#include "calc.c"

#define PRINT_LIMIT 1000

int main() {
    Stack infix; infix.top = NULL;
    Stack postfix; postfix.top = NULL;

    FILE *fp = fopen("input.txt","r");
    if (fp == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    node *inp = new_list();
    while (1) {
        char c = getc(fp);
        if (c == EOF) break;
        push_back(inp, c);
    }
    fclose(fp);

    list_tokenize(inp, &infix);

    // while (!stempty(&infix)) {
    //     if (is_num(peek(&infix))) print_fraction(pop_num(&infix), -1);
    //     else printf("%c", pop_op(&infix));
    //     printf("\n");
    // }
    // return 0;
    
    infix_to_postfix(&infix, &postfix);


    fraction *res = calculate(&postfix);
    print_fraction(res, PRINT_LIMIT);
    free_fraction(res);
    free_list(inp);

    return 0;
}