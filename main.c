#include "calc.c"

int main() {
    char str[] = "-23456789876.5432*5765752765127.357+(-2131231.0-2121241)/2+0";
    // char str[] = "-23456789876.5432*5765752765127.357+(-2131231.0-2121241)/2(3+0)";
    Stack infix; infix.top = NULL;
    Stack postfix; postfix.top = NULL;

    tokenize(str, &infix);
    infix_to_postfix(&infix, &postfix);
    fraction *res = calculate(&postfix);
    print_fraction(res, 500);

    return 0;
}