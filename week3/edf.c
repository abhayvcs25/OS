#include <stdio.h>

#define MAX 10
#define MAX_TIME 1000

typedef struct {
    int id;
    int bt;
    int period;

    int remaining;
    int next_arrival;

    int deadline;   // absolute deadline
    int ct, wt, tat;
} Process;

// GCD
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// LCM
int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

int main() {
    Process p[MAX];
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int hyperperiod = 1;
    float U = 0;

    // Input
    for (int i = 0; i < n; i++) {
        p[i].id = i;

        printf("\nProcess %d:\n", i);
        printf("Burst Time: ");
        scanf("%d", &p[i].bt);

        printf("Period : ");
        scanf("%d", &p[i].period);

        printf("DeadLine : ");
        scanf("%d", &p[i].deadline);

        p[i].remaining = 0;
        p[i].next_arrival = 0;
        p[i].ct = -1;

        hyperperiod = lcm(hyperperiod, p[i].period);

        U += (float)p[i].bt / p[i].period;
    }

    printf("\nUtilization U = %.3f\n", U);

    // ❌ If overload → stop
    if (U > 1.0) {
        printf("Not Schedulable (U > 1)\n");
        return 0;
    }

    int gantt[MAX_TIME];

    // EDF Simulation
    for (int time = 0; time < hyperperiod; time++) {

        // Arrival of tasks
        for (int i = 0; i < n; i++) {
            if (time == p[i].next_arrival) {
                p[i].remaining = p[i].bt;
                p[i].deadline = time + p[i].period; // absolute deadline
                p[i].next_arrival += p[i].period;
            }
        }

        int selected = -1;

        // Select earliest deadline
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0) {
                if (selected == -1 || p[i].deadline < p[selected].deadline)
                    selected = i;
            }
        }

        if (selected != -1) {
            gantt[time] = p[selected].id;
            p[selected].remaining--;

            if (p[selected].remaining == 0)
                p[selected].ct = time + 1;
        } else {
            gantt[time] = -1;
        }
    }

    // Gantt Chart
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < hyperperiod; i++) {
        if (gantt[i] == -1)
            printf(" - |");
        else
            printf(" P%d |", gantt[i]);
    }

    printf("\n ");
    for (int i = 0; i <= hyperperiod; i++)
        printf("%2d  ", i);

    // Final Table
    printf("\n\nID\tBT\tPeriod\tDeadline\tCT\tWT\tTAT\n");

    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct;
        p[i].wt = p[i].tat - p[i].bt;

        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
               p[i].id,
               p[i].bt,
               p[i].period,
               p[i].period,   // relative deadline
               p[i].ct,
               p[i].wt,
               p[i].tat);
    }

    return 0;
}
