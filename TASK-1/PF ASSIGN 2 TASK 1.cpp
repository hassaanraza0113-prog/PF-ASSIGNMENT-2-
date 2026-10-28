#include <stdio.h>

int main(void)
{
    int seats[50][50];
    int freeSeats[50];
    int rows, cols, i, j, k, totalFree = 0;
    int middle, distance, row, start, count;
    int found = 0, chosenRow = -1, chosenStart = -1;

    printf("Enter number of rows (max 50): ");
    scanf("%d", &rows);
    printf("Enter seats per row (max 50): ");
    scanf("%d", &cols);

    if (rows < 1 || rows > 50 || cols < 1 || cols > 50) {
        printf("Invalid seating size.\n");
        return 0;
    }

    for (i = 0; i < rows; i++) {
        printf("Enter seats for row %d (0 free, 1 booked):\n", i);
        for (j = 0; j < cols; j++) {
            do {
                scanf("%d", &seats[i][j]);
                if (seats[i][j] != 0 && seats[i][j] != 1)
                    printf("Enter only 0 or 1: ");
            } while (seats[i][j] != 0 && seats[i][j] != 1);
        }
    }

    for (i = 0; i < rows; i++) {
        freeSeats[i] = 0;
        for (j = 0; j < cols; j++)
            if (seats[i][j] == 0)
                freeSeats[i]++;
        totalFree += freeSeats[i];
    }

    printf("\nFree seats per row: ");
    for (i = 0; i < rows; i++)
        printf("%d ", freeSeats[i]);
    printf(" Total free: %d\n", totalFree);

    printf("\nSeating plan (row and seat numbers start at 0):\n   ");
    for (j = 0; j < cols; j++) printf("%d ", j);
    printf("\n");
    for (i = 0; i < rows; i++) {
        printf("R%d ", i);
        for (j = 0; j < cols; j++)
            printf("%c ", seats[i][j] == 0 ? '-' : 'X');
        printf("\n");
    }

    printf("\nEnter group size: ");
    scanf("%d", &k);
    if (k < 1 || k > cols) {
        printf("Invalid group size.\n");
        return 0;
    }

    middle = rows / 2;

    /* Search middle row, then rows by increasing distance.
       At the same distance, search the smaller row number first. */
    for (distance = 0; distance < rows && !found; distance++) {
        for (i = 0; i < 2 && !found; i++) {
            if (distance == 0 && i == 1)
                continue;
            row = (i == 0) ? middle - distance : middle + distance;
            if (row < 0 || row >= rows)
                continue;

            count = 0;
            for (j = 0; j < cols; j++) {
                if (seats[row][j] == 0)
                    count++;
                else
                    count = 0;

                if (count == k) {
                    chosenRow = row;
                    chosenStart = j - k + 1;
                    found = 1;
                    break;
                }
            }
        }
    }

    if (!found) {
        printf("Sorry, no row has %d consecutive free seats.\n", k);
    } else {
        for (j = chosenStart; j < chosenStart + k; j++)
            seats[chosenRow][j] = 1;

        printf("Group of %d seated in Row %d, seats ", k, chosenRow);
        for (j = chosenStart; j < chosenStart + k; j++)
            printf("%d ", j);
        printf("\n");

        totalFree = 0;
        printf("\nUpdated seating plan:\n   ");
        for (j = 0; j < cols; j++) printf("%d ", j);
        printf("\n");
        for (i = 0; i < rows; i++) {
            freeSeats[i] = 0;
            printf("R%d ", i);
            for (j = 0; j < cols; j++) {
                printf("%c ", seats[i][j] == 0 ? '-' : 'X');
                if (seats[i][j] == 0) freeSeats[i]++;
            }
            totalFree += freeSeats[i];
            printf("\n");
        }
        printf("Remaining free seats: %d\n", totalFree);
    }

    return 0;
}
\
