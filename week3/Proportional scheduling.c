#include <stdio.h>

#define MAX 10

typedef struct {
    int id;
    int weight;
    float share;
    float allocated_time;
} Process;

int main() {
    int n, total_weight = 0;
    float total_cpu_time;

    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter total CPU time available: ");
    scanf("%f", &total_cpu_time);

    // Input weights
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter weight for process %d: ", p[i].id);
        scanf("%d", &p[i].weight);
        total_weight += p[i].weight;
    }

    // Calculate proportional share
    for (int i = 0; i < n; i++) {
        p[i].share = (float)p[i].weight / total_weight;
        p[i].allocated_time = p[i].share * total_cpu_time;
    }

    // Output results
    printf("\nProcess\tWeight\tShare\tAllocated CPU Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%.2f\t%.2f\n",
               p[i].id,
               p[i].weight,
               p[i].share,
               p[i].allocated_time);
    }

    return 0;
}
