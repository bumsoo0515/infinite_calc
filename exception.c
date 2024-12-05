#include <stdio.h>

void ZeroDivision() {
    printf("Cannot divide by 0");
    exit(1);
}

void InvalidInput() {
    printf("Invalid input");
    exit(1);
}

void TypeNotMatch() {
    printf("Data type(operator, number) does not match (Invalid input)");
    exit(1);
}

void ParentheseNotMatch() {
    printf("Opening and closing parentheses do not match");
    exit(1);
}

void OperandMissing() {
    printf("Not enough operands (Invalid input)");
    exit(1);
}

void AllocateFailed() {
    printf("Failed to allocate memory");
    exit(1);
}

// void AccessEmpty() {
//     printf("Tried to access empty container");
//     exit(1);
// }