#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Convert month string to number (1 = Jan, 12 = Dec)
int monthToNum(const char *month) {
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    // Loop through the array of month names to find a match
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(month, months[i]) == 0) return i + 1;
    }
    return -1;  // Return -1 if month is not valid
}

// Validate month by checking if it's a valid month name
int isValidMonth(const char *month) {
    return monthToNum(month) != -1;  // Return true if month is valid
}

// Validate the complete date (month, day, year)
int isValidDate(const char *month, int day, int year) {
    if (!isValidMonth(month)) return 0;  // Return false if the month is invalid
    if (year < MIN_YEAR || year > MAX_YEAR) return 0;  // Check if the year is in a valid range
    if (day < 1 || day > 31) return 0;  // Check if the day is within a valid range
    return 1;  // Return true if the date is valid
}

// Compare two students based on several criteria (year, month, day, last name, etc.)
int compareStudents(const Student *a, const Student *b) {
    if (a->year != b->year) return a->year - b->year;  // Compare by year
    int monthDiff = monthToNum(a->month) - monthToNum(b->month);  // Compare by month
    if (monthDiff != 0) return monthDiff;  // If months differ, return the difference
    if (a->day != b->day) return a->day - b->day;  // Compare by day if year and month are equal
    int lastNameCmp = strcasecmp(a->lastName, b->lastName);  // Compare by last name
    if (lastNameCmp != 0) return lastNameCmp;
    int firstNameCmp = strcasecmp(a->firstName, b->firstName);  // Compare by first name
    if (firstNameCmp != 0) return firstNameCmp;
    if (a->gpa != b->gpa) return (a->gpa < b->gpa) ? 1 : -1;  // Compare by GPA
    if (a->toefl != b->toefl) {  // Compare by TOEFL score if available
        if (a->toefl == -1) return -1;  // Domestic students without TOEFL score come first
        if (b->toefl == -1) return 1;
        return (a->toefl < b->toefl) ? 1 : -1;  // Otherwise compare TOEFL scores
    }
    return (b->status == 'D') - (a->status == 'D');  // Compare by status ('D' before 'I')
}

// Merge two subarrays for merge sort
void merge(Student arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    // Dynamically allocate memory for temporary subarrays
    Student *L = malloc(n1 * sizeof(Student)), *R = malloc(n2 * sizeof(Student));
    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }
    // Copy data to temporary subarrays L[] and R[]
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    // Merge the subarrays into arr[]
    while (i < n1 && j < n2) arr[k++] = compareStudents(&L[i], &R[j]) <= 0 ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];  // Copy remaining elements from L[]
    while (j < n2) arr[k++] = R[j++];  // Copy remaining elements from R[]

    free(L); free(R);  // Free allocated memory for temporary subarrays
}

// Merge sort to sort an array of students
void mergeSort(Student arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);  // Recursively sort the left half
        mergeSort(arr, mid + 1, right);  // Recursively sort the right half
        merge(arr, left, mid, right);  // Merge the sorted halves
    }
}

// Parse a line of text to populate a Student struct with student data
int parseLine(char *line, Student *student) {
    char monthStr[MAX_MONTH_LEN];  // Temporary variable for the month
    int day, year, toeflScore = -1;  // Variables for day, year, and TOEFL score
    // Parse the line into individual fields
    int result = sscanf(line, "%s %s %3s-%d-%d %f %c %d",
                        student->firstName, student->lastName,
                        monthStr, &day, &year, &student->gpa,
                        &student->status, &toeflScore);

    // Handle cases based on the number of parsed fields and student status
    if (result == 8 && student->status == 'I') {
        if (toeflScore < MIN_TOEFL || toeflScore > MAX_TOEFL) {
            fprintf(stderr, "Error: Invalid TOEFL score.\n");
            return 0;  // Return false if TOEFL score is invalid
        }
        student->toefl = toeflScore;
    } else if (result == 7 && student->status == 'D') {
        student->toefl = -1;  // Domestic students don't have a TOEFL score
    } else if (result == 7 && student->status == 'I') {
        student->toefl = 0;  // International students without TOEFL score
    } else {
        fprintf(stderr, "Error: Invalid input format.\n");
        return 0;  // Return false if input format is invalid
    }

    // Validate the date for the student
    if (!isValidDate(monthStr, day, year)) {
        fprintf(stderr, "Error: Invalid date.\n");
        return 0;  // Return false if the date is invalid
    }

    // Copy the valid month, day, and year into the student struct
    strcpy(student->month, monthStr);
    student->day = day;
    student->year = year;

    // Validate the GPA
    if (student->gpa < 0.0 || student->gpa > 4.301) {
        fprintf(stderr, "Error: Invalid GPA.\n");
        return 0;  // Return false if GPA is invalid
    }

    return 1;  // Return true if the line was successfully parsed and is valid
}
