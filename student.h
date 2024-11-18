#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

// Maximum length constants for various strings
#define MAX_NAME_LEN 100    // Maximum length for first and last names
#define MAX_LINE_LEN 200    // Maximum length for a line in a file or input
#define MAX_MONTH_LEN 4     // Maximum length for month name (e.g., "Jan")
#define MIN_YEAR 1950       // Minimum valid year for student enrollment
#define MAX_YEAR 2010       // Maximum valid year for student enrollment
#define MAX_TOEFL 120       // Maximum TOEFL score
#define MIN_TOEFL 0         // Minimum TOEFL score

// Struct to represent a student
typedef struct {
    char firstName[MAX_NAME_LEN];   // Student's first name
    char lastName[MAX_NAME_LEN];    // Student's last name
    char month[MAX_MONTH_LEN];      // Month of birth (e.g., "Jan")
    int day;                        // Day of birth
    int year;                       // Year of birth
    float gpa;                      // Grade Point Average of the student
    char status;                    // Enrollment status ('A' for active, 'I' for inactive)
    int toefl;                      // TOEFL score, -1 for domestic students (no TOEFL required)
} Student;

// Function declarations
// Converts month name to numeric representation (e.g., "Jan" -> 1)
int monthToNum(const char *month);

// Validates if the provided month name is correct
int isValidMonth(const char *month);

// Validates if the provided date (month, day, year) is a valid date
int isValidDate(const char *month, int day, int year);

// Compares two students based on their GPA or other attributes (used for sorting)
int compareStudents(const Student *a, const Student *b);

// Sorts an array of students using the merge sort algorithm
void mergeSort(Student arr[], int left, int right);

// Parses a line of text and populates a Student struct with the data
int parseLine(char *line, Student *student);

#endif // STUDENT_H
