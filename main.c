#include <stdio.h>

void *novaThread(void* args) {
    printf("Created a new thread");
}

int main () {

    printf("Hello world");
    return 0;
}