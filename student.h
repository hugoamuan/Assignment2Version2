#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define MAX_NAME_LEN 100
#define MAX_LINE_LEN 200
#define MAX_MONTH_LEN 4
#define MIN_YEAR 1950
#define MAX_YEAR 2010
#define MAX_TOEFL 120
#define MIN_TOEFL 0

typedef struct {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    char month[MAX_MONTH_LEN];
    int day;
    int year;
    float gpa;
    char status;
    int toefl;  // -1 for domestic students
} Student;

// Function declarations
int monthToNum(const char *month);
int isValidMonth(const char *month);
int isValidDate(const char *month, int day, int year);
int compareStudents(const Student *a, const Student *b);
void mergeSort(Student arr[], int left, int right);
int parseLine(char *line, Student *student);

#endif // STUDENT_H
