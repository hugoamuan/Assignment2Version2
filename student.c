#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Convert month string to number
int monthToNum(const char *month) {
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(month, months[i]) == 0) return i + 1;
    }
    return -1;
}

// Validate month
int isValidMonth(const char *month) {
    return monthToNum(month) != -1;
}

// Validate date
int isValidDate(const char *month, int day, int year) {
    if (!isValidMonth(month)) return 0;
    if (year < MIN_YEAR || year > MAX_YEAR) return 0;
    if (day < 1 || day > 31) return 0;
    return 1;
}

// Compare students based on criteria
int compareStudents(const Student *a, const Student *b) {
    if (a->year != b->year) return a->year - b->year;
    int monthDiff = monthToNum(a->month) - monthToNum(b->month);
    if (monthDiff != 0) return monthDiff;
    if (a->day != b->day) return a->day - b->day;
    int lastNameCmp = strcasecmp(a->lastName, b->lastName);
    if (lastNameCmp != 0) return lastNameCmp;
    int firstNameCmp = strcasecmp(a->firstName, b->firstName);
    if (firstNameCmp != 0) return firstNameCmp;
    if (a->gpa != b->gpa) return (a->gpa < b->gpa) ? 1 : -1;
    if (a->toefl != b->toefl) {
        if (a->toefl == -1) return -1;
        if (b->toefl == -1) return 1;
        return (a->toefl < b->toefl) ? 1 : -1;
    }
    return (b->status == 'D') - (a->status == 'D');
}

// Merge for merge sort
void merge(Student arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    Student *L = malloc(n1 * sizeof(Student)), *R = malloc(n2 * sizeof(Student));
    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = compareStudents(&L[i], &R[j]) <= 0 ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

// Merge sort
void mergeSort(Student arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parse line for student data
int parseLine(char *line, Student *student) {
    char monthStr[MAX_MONTH_LEN];
    int day, year, toeflScore = -1;
    int result = sscanf(line, "%s %s %3s-%d-%d %f %c %d",
                        student->firstName, student->lastName,
                        monthStr, &day, &year, &student->gpa,
                        &student->status, &toeflScore);

    if (result == 8 && student->status == 'I') {
        if (toeflScore < MIN_TOEFL || toeflScore > MAX_TOEFL) {
            fprintf(stderr, "Error: Invalid TOEFL score.\n");
            return 0;
        }
        student->toefl = toeflScore;
    } else if (result == 7 && student->status == 'D') {
        student->toefl = -1;
    } else if (result == 7 && student->status == 'I') {
        student->toefl = 0;
    } else {
        fprintf(stderr, "Error: Invalid input format.\n");
        return 0;
    }

    if (!isValidDate(monthStr, day, year)) {
        fprintf(stderr, "Error: Invalid date.\n");
        return 0;
    }

    strcpy(student->month, monthStr);
    student->day = day;
    student->year = year;

    if (student->gpa < 0.0 || student->gpa > 4.301) {
        fprintf(stderr, "Error: Invalid GPA.\n");
        return 0;
    }

    return 1;
}
