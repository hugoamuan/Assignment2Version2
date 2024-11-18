#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "student.c"

/**
 * This program reads student information from an input file, filters
 * the students based on a specified status option, sorts the list, and writes
 * the sorted output to a specified file.
 *
 * @author Hugo Amuan & Mitchell MacDonald
 * @version 1.0
 */

/**
 * Main function to process student data.
 *
 * This program reads student information from an input file, filters
 * students based on the specified status option (graduated, international, or both),
 * sorts the list of students, and writes the sorted information to an output file.
 *
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line arguments:
 *   - argv[1]: The input file containing student data.
 *   - argv[2]: The output file to write processed student data.
 *   - argv[3]: The filter option (1 for graduated, 2 for international, 3 for both).
 *
 * @return 0 if successful, 1 if an error occurs.
 */
int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <option>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];  // The input file with student data.
    const char *outputFile = argv[2]; // The output file to save processed data.
    int option = atoi(argv[3]);       // The filter option (1, 2, or 3).

    // Validate the filter option.
    if (option < 1 || option > 3) {
        fprintf(stderr, "Error: Invalid option. Must be 1, 2, or 3.\n");
        return 1;
    }

    // Open the input file for reading.
    FILE *in = fopen(inputFile, "r");
    if (!in) {
        fprintf(stderr, "Error: Could not open input file.\n");
        return 1;
    }

    // Open the output file for writing.
    FILE *out = fopen(outputFile, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file.\n");
        fclose(in);
        return 1;
    }

    // Allocate memory for an array of Student structs.
    Student *students = malloc(100 * sizeof(Student));
    if (!students) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    char line[MAX_LINE_LEN];  // Buffer to read each line from the input file.
    size_t count = 0, capacity = 100;  // Track number of students and allocated memory.

    // Read each line from the input file.
    while (fgets(line, sizeof(line), in)) {
        Student student;

        // Parse the student data from the line.
        if (!parseLine(line, &student)) {
            fprintf(out, "Error: Invalid input format in line: %s\n", line);
            free(students);
            fclose(in);
            fclose(out);
            return 1;
        }

        // Filter students based on the selected status option.
        if ((option == 1 && student.status != 'D') ||  // Graduated students (status 'D')
            (option == 2 && student.status != 'I')) { // International students (status 'I')
            continue;
        }

        // Resize the students array if the current count exceeds capacity.
        if (count == capacity) {
            capacity *= 2; // Double the capacity.
            Student *temp = realloc(students, capacity * sizeof(Student));
            if (!temp) {
                fprintf(stderr, "Memory allocation failed\n");
                free(students);
                fclose(in);
                fclose(out);
                return 1;
            }
            students = temp;
        }

        // Add the student to the array.
        students[count++] = student;
    }

    // Close the input file after reading.
    fclose(in);

    // Sort the students array using merge sort.
    mergeSort(students, 0, count - 1);

    // Write the sorted student data to the output file.
    for (size_t i = 0; i < count; i++) {
        Student *s = &students[i];
        // Print student data (name, date, GPA, status, TOEFL score if applicable).
        fprintf(out, "%s %s %s-%d-%d %.3f %c", s->firstName, s->lastName, s->month, s->day, s->year, s->gpa, s->status);
        if (s->status == 'I') fprintf(out, " %d", s->toefl);  // Include TOEFL score for international students.
        fprintf(out, "\n");
    }

    // Free the allocated memory for students array and close the output file.
    free(students);
    fclose(out);

    return 0;  // Return success.
}
