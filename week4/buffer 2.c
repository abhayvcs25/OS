#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int *buffer;
int size, n;   // buffer size, number of items
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

// Print buffer
void printBuffer() {
    printf("Buffer[");
    for (int i = 0; i < size; i++) {
        printf("%d", buffer[i]);
        if (i != size - 1) printf(" ");
    }
    printf("]\n");
}

// Producer
void *producer(void *arg) {
    for (int i = 1; i <= n; i++) {

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = i;
        printf("Produced: %d | ", i);
        printBuffer();

        in = (in + 1) % size;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

// Consumer
void *consumer(void *arg) {
    int item;

    for (int i = 1; i <= n; i++) {

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        buffer[out] = 0;

        printf("Consumed: %d | ", item);
        printBuffer();

        out = (out + 1) % size;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    printf("Enter buffer size: ");
    scanf("%d", &size);

    printf("Enter number of items to produce: ");
    scanf("%d", &n);

    buffer = (int *)malloc(size * sizeof(int));

    // Initialize buffer
    for (int i = 0; i < size; i++)
        buffer[i] = 0;

    sem_init(&empty, 0, size);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer);

    return 0;
}
