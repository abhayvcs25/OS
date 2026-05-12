#include <stdio.h>

int main() {
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], type[n];
    int ct[n], tat[n], wt[n];
    int completed[n];


    for(int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i+1);
        pid[i] = i + 1;

        printf("Arrival Time: ");
        scanf("%d", &at[i]);

        printf("Burst Time: ");
        scanf("%d", &bt[i]);

        printf("Type (0 = System, 1 = User): ");
        scanf("%d", &type[i]);

        completed[i] = 0;
    }

    int time = 0, done = 0;

    while(done < n) {
        int idx = -1;


        for(int i = 0; i < n; i++) {
            if(type[i] == 0 && completed[i] == 0 && at[i] <= time) {
                if(idx == -1 || at[i] < at[idx]) {
                    idx = i;
                }
            }
        }


        if(idx == -1) {
            for(int i = 0; i < n; i++) {
                if(type[i] == 1 && completed[i] == 0 && at[i] <= time) {
                    if(idx == -1 || at[i] < at[idx]) {
                        idx = i;
                    }
                }
            }
        }


        if(idx != -1) {
            if(time < at[idx])
                time = at[idx];

            time += bt[idx];
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];

            completed[idx] = 1;
            done++;
        } else {
            time++;
        }
    }

    printf("\nPID\tAT\tBT\tTYPE\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               pid[i], at[i], bt[i], type[i],
               ct[i], tat[i], wt[i]);
    }

    return 0;
}

