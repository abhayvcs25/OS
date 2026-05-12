#include <stdio.h>

#define MAX 100

// Queues
int sysQ[MAX], userQ[MAX];
int f1 = -1, r1 = -1;
int f2 = -1, r2 = -1;

void enqueue_sys(int x) {
    if (f1 == -1) f1 = 0;
    sysQ[++r1] = x;
}

int dequeue_sys() {
    int val = sysQ[f1];
    if (f1 == r1) f1 = r1 = -1;
    else f1++;
    return val;
}

int isEmpty_sys() {
    return f1 == -1;
}

void enqueue_user(int x) {
    if (f2 == -1) f2 = 0;
    userQ[++r2] = x;
}

int dequeue_user() {
    int val = userQ[f2];
    if (f2 == r2) f2 = r2 = -1;
    else f2++;
    return val;
}

int isEmpty_user() {
    return f2 == -1;
}

int main() {
    int n;
    int pid[MAX], at[MAX], bt[MAX], rt[MAX], type[MAX];

    int ct[MAX], tat[MAX], wt[MAX], rt_time[MAX], first_exec[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        pid[i]=i;
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        printf("Type (1=System, 2=User): ");
        scanf("%d", &type[i]);

        rt[i] = bt[i];
        first_exec[i] = -1;
    }

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (at[i] > at[j]) {
                int temp;

                temp = at[i]; at[i] = at[j]; at[j] = temp;
                temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                temp = rt[i]; rt[i] = rt[j]; rt[j] = temp;
                temp = pid[i]; pid[i] = pid[j]; pid[j] = temp;
                temp = type[i]; type[i] = type[j]; type[j] = temp;
            }
        }
    }

    int time = 0, completed = 0, i = 0;
    int current = -1;

    while (completed < n) {

        // Add arrived processes
        while (i < n && at[i] <= time) {
            if (type[i] == 1)
                enqueue_sys(i);
            else
                enqueue_user(i);
            i++;
        }

        // Preemption: system interrupts user
        if (current != -1 && type[current] == 2 && !isEmpty_sys()) {
            enqueue_user(current);
            current = -1;
        }

        // Select process (System priority)
        if (current == -1) {
            if (!isEmpty_sys())
                current = dequeue_sys();
            else if (!isEmpty_user())
                current = dequeue_user();
            else {
                time++;
                continue;
            }
        }

        // First response time
        if (first_exec[current] == -1) {
            first_exec[current] = time;
            rt_time[current] = time - at[current];
        }

        // Execute (FCFS → run until completion unless preempted)
        rt[current]--;
        time++;

        // Completion
        if (rt[current] == 0) {
            ct[current] = time;
            tat[current] = ct[current] - at[current];
            wt[current] = tat[current] - bt[current];

            completed++;
            current = -1;
        }
    }

    // Output Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    float avg_tat = 0, avg_wt = 0, avg_rt = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i],
               ct[i], tat[i], wt[i], rt_time[i]);

        avg_tat += tat[i];
        avg_wt += wt[i];
        avg_rt += rt_time[i];
    }

    printf("\nAverage TAT = %.2f", avg_tat / n);
    printf("\nAverage WT  = %.2f", avg_wt / n);
    printf("\nAverage RT  = %.2f\n", avg_rt / n);

    return 0;
}
