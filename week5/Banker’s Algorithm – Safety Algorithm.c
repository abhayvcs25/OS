#include <stdio.h>

#define MAX 10

// Safety Algorithm
int isSafe(int n, int m, int alloc[MAX][MAX], int need[MAX][MAX],
           int avail[MAX], int safeSeq[]) {

    int work[MAX], finish[MAX] = {0};

    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    int count = 0;

    while (count < n) {
        int found = 0;

        for (int i = 0; i < n; i++) {

            if (finish[i] == 0) {

                int j;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j])
                        break;
                }

                if (j == m) {

                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (found == 0)
            return 0;
    }

    return 1;
}

int main() {

    int n, m;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[MAX][MAX], max[MAX][MAX], need[MAX][MAX];
    int avail[MAX], safeSeq[MAX];

    // Allocation matrix
    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    // Max matrix
    printf("\nEnter Maximum Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    // Available vector
    printf("\nEnter Available Resources:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Need matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Initial safety check
    if (!isSafe(n, m, alloc, need, avail, safeSeq)) {
        printf("\nSystem is NOT in safe state.\n");
        return 0;
    }

    printf("\nSystem is SAFE.\nSafe Sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);

    // Resource request
    int pid;
    int request[MAX];

    printf("\n\nEnter process id (0 to %d): ", n - 1);
    scanf("%d", &pid);

    printf("Enter resource request: ");
    for (int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    // Check Request <= Need
    for (int i = 0; i < m; i++) {
        if (request[i] > need[pid][i]) {
            printf("\nError: Request exceeds need.\n");
            return 0;
        }
    }

    // Check Request <= Available
    for (int i = 0; i < m; i++) {
        if (request[i] > avail[i]) {
            printf("\nResources not available.\n");
            return 0;
        }
    }

    // Temporary allocation
    for (int i = 0; i < m; i++) {
        avail[i] -= request[i];
        alloc[pid][i] += request[i];
        need[pid][i] -= request[i];
    }

    // Check safe state again
    if (isSafe(n, m, alloc, need, avail, safeSeq)) {

        printf("\nRequest can be GRANTED.\n");
        printf("New Safe Sequence: ");

        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
    }
    else {

        // Rollback
        for (int i = 0; i < m; i++) {
            avail[i] += request[i];
            alloc[pid][i] -= request[i];
            need[pid][i] += request[i];
        }

        printf("\nRequest CANNOT be granted (unsafe state).\n");
    }

    return 0;
}
