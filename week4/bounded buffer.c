#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 100

char buffer[MAX];
int in = 0, out = 0;
int BUFFER_SIZE;

// Semaphores
sem_t empty, full, mutex;

// Control semaphores
sem_t prod_signal, cons_signal;

// Completion semaphore (IMPORTANT FIX)
sem_t done;

// Print lock
pthread_mutex_t print_lock;

int step = 1;
char next_item;

// Print buffer
void print_buffer() {
    printf("[");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%c", buffer[i]);
        if (i != BUFFER_SIZE - 1) printf(", ");
    }
    printf("]");
}

// Print table row
void print_state(char *operation) {
    pthread_mutex_lock(&print_lock);

    int e, f, m;
    sem_getvalue(&empty, &e);
    sem_getvalue(&full, &f);
    sem_getvalue(&mutex, &m);

    printf("%-5d %-25s ", step++, operation);
    print_buffer();
    printf("  %-3d %-3d %-5d %-4d %-5d\n", in, out, e, f, m);

    pthread_mutex_unlock(&print_lock);
}

// Producer thread
void* producer(void* arg) {
    while (1) {
        sem_wait(&prod_signal);

        if (sem_trywait(&empty) != 0) {
            print_state("Producer FULL");
            sem_post(&done);   // notify main
            continue;
        }

        sem_wait(&mutex);

        buffer[in] = next_item;
        char curr = next_item;
        in = (in + 1) % BUFFER_SIZE;

        char op[50];
        sprintf(op, "Producer inserts %c", curr);
        print_state(op);

        sem_post(&mutex);
        sem_post(&full);

        sem_post(&done);   // notify main
    }
}

// Consumer thread
void* consumer(void* arg) {
    while (1) {
        sem_wait(&cons_signal);

        if (sem_trywait(&full) != 0) {
            print_state("Consumer EMPTY");
            sem_post(&done);   // notify main
            continue;
        }

        sem_wait(&mutex);

        char item = buffer[out];
        buffer[out] = '_';
        out = (out + 1) % BUFFER_SIZE;

        char op[50];
        sprintf(op, "Consumer removes %c", item);
        print_state(op);

        sem_post(&mutex);
        sem_post(&empty);

        sem_post(&done);   // notify main
    }
}

int main() {
    pthread_t p, c;
    int choice;

    printf("Enter BUFFER SIZE: ");
    scanf("%d", &BUFFER_SIZE);

    // Initialize buffer
    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '_';

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    sem_init(&prod_signal, 0, 0);
    sem_init(&cons_signal, 0, 0);

    sem_init(&done, 0, 0);  // IMPORTANT

    pthread_mutex_init(&print_lock, NULL);

    printf("\nStep  Operation                Buffer        in  out empty full mutex\n");
    printf("-----------------------------------------------------------------------\n");

    print_state("Initial");

    // Create threads
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    // User loop
    while (1) {
        pthread_mutex_lock(&print_lock);
        printf("\n1. Produce\n2. Consume\n0. Exit\nEnter choice: ");
        pthread_mutex_unlock(&print_lock);

        scanf("%d", &choice);

        if (choice == 1) {
            pthread_mutex_lock(&print_lock);
            printf("Enter item: ");
            pthread_mutex_unlock(&print_lock);

            scanf(" %c", &next_item);

            sem_post(&prod_signal);
            sem_wait(&done);   // WAIT → ensures table prints first
        }
        else if (choice == 2) {
            sem_post(&cons_signal);
            sem_wait(&done);   // WAIT → ensures table prints first
        }
        else if (choice == 0) {
            exit(0);
        }
        else {
            pthread_mutex_lock(&print_lock);
            printf("Invalid choice\n");
            pthread_mutex_unlock(&print_lock);
        }
    }

    return 0;
}
