#include "calc.c"

int main() {
    Stack infix; infix.top = NULL;
    Stack postfix; postfix.top = NULL;

    FILE *fp = fopen("input.txt","r");

    node *inp = new_list();
    while (1) {
        char c = getc(fp);
        if (c == EOF) break;
        push_back(inp, c);
    }

    list_tokenize(inp, &infix);
    infix_to_postfix(&infix, &postfix);
    fraction *res = calculate(&postfix);
    print_fraction(res, LIMIT);

    return 0;
}