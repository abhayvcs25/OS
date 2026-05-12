#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define SIZE 5

int buffer[SIZE];
int top = -1;

sem_t mutex, empty, full;

void produce() {
    int item;

    sem_wait(&empty);
    sem_wait(&mutex);

    item = top + 2;   // Just for display: Item 1, 2, 3...
    top++;
    buffer[top] = item;

    printf("Producer has produced: Item %d\n", item);

    sem_post(&mutex);
    sem_post(&full);
}

void consume() {
    sem_wait(&full);
    sem_wait(&mutex);

    int item = buffer[top];
    printf("Consumer has consumed: Item %d\n", item);
    top--;

    sem_post(&mutex);
    sem_post(&empty);
}

int main() {
    int choice;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);

    while (1) {
        printf("\nEnter 1.Producer 2.Consumer 3.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (top == SIZE - 1) {
                    printf("Buffer is full!\n");
                } else {
                    produce();
                }
                break;

            case 2:
                if (top == -1) {
                    printf("Buffer is empty!\n");
                } else {
                    consume();
                }
                break;

            case 3:
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
