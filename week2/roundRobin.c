#include <stdio.h>

int main() {
    int n, tq;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[n], bt[n], rt[n],pri[n];
    int ct[n], wt[n], tat[n], resp[n];
    int visited[n];

    printf("Enter Arrival Time and Burst Time and priority:\n");
    for(int i = 0; i < n; i++) {
        printf("P%d AT BT: ", i+1);
        scanf("%d %d %d", &at[i], &bt[i], &pri[i]);
        rt[i] = bt[i];
        visited[i] = 0;
        resp[i] = -1;  // response time tracking
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int queue[100], front = 0, rear = 0;

    int time = 0, completed = 0;

    // Start from first arriving process
    queue[rear++] = 0;
    visited[0] = 1;

    while(completed < n) {
        int i = queue[front++];

        if(resp[i] == -1) {
            resp[i] = time - at[i];
        }

        if(rt[i] > tq) {
            printf("|p%d",i+1);
            time += tq;
            rt[i] -= tq;
        } else {
            time += rt[i];
            rt[i] = 0;
            ct[i] = time;
            completed++;
        }

        for(int j = 0; j < n; j++) {
            if(at[j] <= time && visited[j] == 0) {
                queue[rear++] = j;
                visited[j] = 1;
            }
        }

        if(rt[i] > 0) {
            queue[rear++] = i;
        }

        if(front == rear) {
            for(int j = 0; j < n; j++) {
                if(visited[j] == 0) {
                    queue[rear++] = j;
                    visited[j] = 1;
                    time = at[j];
                    break;
                }
            }
        }
    }

    float total_wt = 0, total_tat = 0, total_rt = 0;

    printf("\nProcess\tAT\tBT\tCT\tWT\tTAT\tRT\n");
    for(int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i+1, at[i], bt[i], ct[i], wt[i], tat[i], resp[i]);

        total_wt += wt[i];
        total_tat += tat[i];
        total_rt += resp[i];
    }

    printf("\nAverage WT = %.2f", total_wt/n);
    printf("\nAverage TAT = %.2f", total_tat/n);
    printf("\nAverage RT = %.2f\n", total_rt/n);

    return 0;
}
