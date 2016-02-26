#include <stdio.h>

struct TestStruct {
    char m1;
    int m2;
}__attribute__((packed, aligned(1)));

int main() {
    printf("sizeof:%d\n", sizeof(struct TestStruct)); 
}
