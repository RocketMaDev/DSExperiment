#include "../src/qsoperation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static void print_stack(ArrayList *stack) {
    int recv;
    while (StackLength(stack)) {
        StackPop(stack, &recv);
        printf("%d", recv);
    }
}

static void tst_conversion(void) {
    puts("=====tst_conversion=====");
    ArrayList buf;
    int err = ArrayListInit(&buf, 0);
    srand(time(NULL));
    unsigned int r = rand() & 0xfff;
    printf("Choosing random number: %d (also %#x)\n", r, r);

    printf("Converting to base of 2: 0b");
    err += Conversion(r, 2, &buf);
    print_stack(&buf);
    puts("");

    printf("Converting to base of 8: 0o");
    ArrayListClear(&buf);
    err += Conversion(r, 8, &buf);
    print_stack(&buf);
    puts("");

    printf("Sum of status: %d\n", err);
    ArrayListRelease(&buf);
    puts("========================\n");
}

static Queue buf;
static void tst_enqueue(void) {
    puts("=====tst_enqueue=====");
    Enqueue(&buf, 1);
    Enqueue(&buf, 2);
    Enqueue(&buf, 3);
    Enqueue(&buf, 4);
    Enqueue(&buf, 5);
    Enqueue(&buf, 6);
    Enqueue(&buf, 7);
    Enqueue(&buf, 8);
    Enqueue(&buf, 9);
    printf("After enqueueing 1..9: front = %d, rear = %d\n", buf.front, buf.rear);
    puts("=====================\n");
}

static void tst_onemore(void) {
    puts("=====tst_onemore=====");
    Enqueue(&buf, 10);
    printf("After enqueueing 10: front = %d, rear = %d\n", buf.front, buf.rear);
    puts("=====================\n");
}

static void tst_dequeue(void) {
    puts("=====tst_dequeue=====");
    Dequeue(&buf, NULL);
    Dequeue(&buf, NULL);
    Dequeue(&buf, NULL);
    Dequeue(&buf, NULL);
    Dequeue(&buf, NULL);
    Enqueue(&buf, 11);
    Enqueue(&buf, 12);
    printf("After dequeueing 1..5 and enqueueing 11..12: length = %d, front = %d, rear = %d\n",
            QueueLength(&buf), buf.front, buf.rear);
    puts("=====================\n");
}

static void tst_reverse(void) {
    puts("=====tst_reverse=====");
    int err = QueueReverse(&buf);
    printf("QueueReverse status: %d\n", err);
    int recv;
    printf("After reversing the queue: [");
    while (QueueLength(&buf)) {
        Dequeue(&buf, &recv);
        printf("%d, ", recv);
    }
    puts("\b\b] ");
    puts("=====================\n");
}

int main(void) {
    tst_conversion();
    tst_enqueue();
    tst_onemore();
    tst_dequeue();
    tst_reverse();
    return 0;
}
