#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5   // Number of philosophers

pthread_mutex_t forks[N];

void* philosopher(void* num) {
    int id = *(int*)num;

    // Thinking
    printf("Philosopher %d is THINKING\n", id);
    sleep(1);

    if (id % 2 == 0) {
        // Even philosopher picks RIGHT first
        pthread_mutex_lock(&forks[(id + 1) % N]);
        printf("Philosopher %d picked RIGHT fork\n", id);

        pthread_mutex_lock(&forks[id]);
        printf("Philosopher %d picked LEFT fork\n", id);
    } else {
        // Odd philosopher picks LEFT first
        pthread_mutex_lock(&forks[id]);
        printf("Philosopher %d picked LEFT fork\n", id);

        pthread_mutex_lock(&forks[(id + 1) % N]);
        printf("Philosopher %d picked RIGHT fork\n", id);
    }

    // Eating (only once)
    printf("Philosopher %d is EATING\n", id);
    sleep(2);

    // Put down forks
    pthread_mutex_unlock(&forks[id]);
    pthread_mutex_unlock(&forks[(id + 1) % N]);

    printf("Philosopher %d finished eating and exits\n", id);

    pthread_exit(NULL);
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    // Initialize mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    printf("\nAll philosophers have eaten once. Program ends.\n");

    return 0;
}
