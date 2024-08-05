#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PLACE_LENGTH 128

typedef struct {
    char time[MAX_LINE_LENGTH];
    double mag;
    double cost;
    char place[MAX_PLACE_LENGTH];
} Earthquake;

void readCSV(const char* filename, Earthquake** earthquakes, int* count);
void calculateStatistics(Earthquake* earthquakes, int count, double* sum, double* avg, double* avg_cost);
void printEarthquakes(Earthquake* earthquakes, int count);
void printStatistics(double sum, int count, double avg, double avg_cost);

int main() {
    const char* filename = "significant_month.csv";
    Earthquake* earthquakes = NULL;
    int count = 0;
    
    readCSV(filename, &earthquakes, &count);

    double sum = 0, avg = 0, avg_cost = 0;
    calculateStatistics(earthquakes, count, &sum, &avg, &avg_cost);

    printEarthquakes(earthquakes, count);
    printStatistics(sum, count, avg, avg_cost);

    free(earthquakes);
    return 0;
}

void readCSV(const char* filename, Earthquake** earthquakes, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int capacity = 10;
    *earthquakes = (Earthquake*)malloc(capacity * sizeof(Earthquake));
    if (!(*earthquakes)) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    fgets(line, MAX_LINE_LENGTH, file); // Skip header line
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (index >= capacity) {
            capacity *= 2;
            *earthquakes = (Earthquake*)realloc(*earthquakes, capacity * sizeof(Earthquake));
            if (!(*earthquakes)) {
                perror("Memory reallocation error");
                exit(EXIT_FAILURE);
            }
        }

        Earthquake eq;
        char* token = strtok(line, ",");
        strcpy(eq.time, token);

        // Skip to the 'mag' (magnitude) column
        for (int i = 0; i < 4; ++i) {
            token = strtok(NULL, ",");
        }
        eq.mag = atof(token);
        eq.cost = eq.mag * 1000000;

        // Skip to the 'place' column
        for (int i = 0; i < 9; ++i) {
            token = strtok(NULL, ",");
        }

        strcpy(eq.place, token);

        (*earthquakes)[index++] = eq;
    }

    *count = index;
    fclose(file);
}

void calculateStatistics(Earthquake* earthquakes, int count, double* sum, double* avg, double* avg_cost) {
    *sum = 0;
    double total_cost = 0;
    for (int i = 0; i < count; ++i) {
        *sum += earthquakes[i].mag;
        total_cost += earthquakes[i].cost;
    }
    *avg = *sum / count;
    *avg_cost = total_cost / count;
}

void printEarthquakes(Earthquake* earthquakes, int count) {
    for (int i = 0; i < count; ++i) {
        printf("Time: %s, Magnitude: %.2f, Cost: $%.2f, Place: \"%s\"\n",
               earthquakes[i].time, earthquakes[i].mag, earthquakes[i].cost, earthquakes[i].place);
    }
}

void printStatistics(double sum, int count, double avg, double avg_cost) {
    printf("\nUSGS - Earthquake Statistics - 2024\n");
    printf("Magnitude Statistics => Sum = %.2f, Count = %d, Average magnitude = %.2f\n", sum, count, avg);
    printf("Average Earthquake cost = $%.2f\n", avg_cost);
}
