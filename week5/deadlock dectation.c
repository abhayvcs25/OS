#include <stdio.h>

#define MAX 10

int main()
{
    int n, m;
    int alloc[MAX][MAX], request[MAX][MAX];
    int safeSeq[MAX], avail[MAX], finish[MAX];

    printf("Enter number of processes = ");
    scanf("%d", &n);

    printf("Enter number of resources = ");
    scanf("%d", &m);

    // initialize finish
    for(int i = 0; i < n; i++)
        finish[i] = 0;

    // Allocation matrix
    printf("\nEnter allocation matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Request matrix
    printf("\nEnter request matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &request[i][j]);
        }
    }

    // Available vector
    printf("\nEnter available resources:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }

    int count = 0;

    while (count < n) {

        int found = 0;

        for (int i = 0; i < n; i++) {

            if (finish[i] == 0) {

                int j;

                for (j = 0; j < m; j++) {
                    if (request[i][j] > avail[j])
                        break;
                }

                // request <= available
                if (j == m) {

                    for (int k = 0; k < m; k++) {
                        avail[k] += alloc[i][k];
                    }

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        // Deadlock detected
        if (found == 0)
            break;
    }

    if (count == n) {

        printf("\nNo Deadlock detected.\n");

        printf("Safe sequence: ");
        for (int i = 0; i < n; i++) {
            printf("P%d ", safeSeq[i]);
        }
    }
    else {

        printf("\nDeadlock detected.\n");

        printf("Processes in deadlock: ");
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0)
                printf("P%d ", i);
        }
        printf("\nSafe sequence: ");
        for (int i = 0; i < n; i++) {
            if (finish[i] != 0)
                printf("P%d ", safeSeq[i]);
        }
    }

    return 0;
}
