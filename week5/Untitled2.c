#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define MAX_EAT 4   // maximum times each philosopher eats

sem_t chopstick[N];
sem_t mutex;

void* philosopher(void* num) {
    int i = *(int*)num;

    for (int count = 0; count < MAX_EAT; count++) {

        printf("P%d is thinking\n", i);
        sleep(1); // simulate thinking

        sem_wait(&mutex); // allow only 4 philosophers

        sem_wait(&chopstick[i]);              // left chopstick
        sem_wait(&chopstick[(i + 1) % N]);    // right chopstick

        printf("P%d is eating (%d time)\n", i, count + 1);
        sleep(1); // simulate eating

        sem_post(&chopstick[i]);              // release left
        sem_post(&chopstick[(i + 1) % N]);    // release right

        sem_post(&mutex); // allow others
    }

    return NULL;
}

int main() {
    pthread_t thread[N];
    int phil[N];

    sem_init(&mutex, 0, N - 1);

    for (int i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 1);
    }

    for (int i = 0; i < N; i++) {
        phil[i] = i;
        pthread_create(&thread[i], NULL, philosopher, &phil[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}
