#include <stdio.h>

#define MAX_SENSORS 50
#define MAX_HOURS 100

void inputReadings(int readings[][MAX_HOURS], int sensors, int hours)
{
    int i, j;

    for (i = 0; i < sensors; i++) {
        printf("Sensor %d readings: ", i + 1);
        for (j = 0; j < hours; j++)
            scanf("%d", &readings[i][j]);
    }
}

float analyzeSensor(int readings[], int hours)
{
    int i, total = 0, highest = readings[0], lowest = readings[0];
    int critical = 0, warning = 0, currentStreak = 0, longestStreak = 0;
    float average;

    for (i = 0; i < hours; i++) {
        total += readings[i];

        if (readings[i] > highest) highest = readings[i];
        if (readings[i] < lowest) lowest = readings[i];

        if (readings[i] >= 40) critical++;
        if (readings[i] >= 35 && readings[i] <= 39) warning++;

        if (readings[i] >= 35) {
            currentStreak++;
            if (currentStreak > longestStreak)
                longestStreak = currentStreak;
        } else {
            currentStreak = 0;
        }
    }

    average = (float) total / hours;
    printf("Total=%d  Avg=%.2f  Max=%d  Min=%d\n",
           total, average, highest, lowest);
    printf("Critical=%d  Warning=%d  Longest hot streak=%d\n",
           critical, warning, longestStreak);

    return average;
}

int highestAverageSensor(float averages[], int sensors)
{
    int i, best = 0;

    for (i = 1; i < sensors; i++) {
        if (averages[i] > averages[best])
            best = i;
    }

    return best;
}

int main(void)
{
    int readings[MAX_SENSORS][MAX_HOURS];
    float averages[MAX_SENSORS];
    int sensors, hours, i, best;

    printf("Enter number of sensors (max 50): ");
    scanf("%d", &sensors);
    printf("Enter number of hourly readings (max 100): ");
    scanf("%d", &hours);

    if (sensors < 1 || sensors > MAX_SENSORS ||
        hours < 1 || hours > MAX_HOURS) {
        printf("Invalid number of sensors or readings.\n");
        return 0;
    }

    inputReadings(readings, sensors, hours);

    for (i = 0; i < sensors; i++) {
        printf("\nSensor %d: ", i + 1);
        averages[i] = analyzeSensor(readings[i], hours);
    }

    best = highestAverageSensor(averages, sensors);
    printf("\nHighest average: Sensor %d (%.2f)\n",
           best + 1, averages[best]);

    return 0;
}


