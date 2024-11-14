#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <option>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = argv[2];
    int option = atoi(argv[3]);
    if (option < 1 || option > 3) {
        fprintf(stderr, "Error: Invalid option. Must be 1, 2, or 3.\n");
        return 1;
    }

    FILE *in = fopen(inputFile, "r");
    if (!in) {
        fprintf(stderr, "Error: Could not open input file.\n");
        return 1;
    }

    FILE *out = fopen(outputFile, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file.\n");
        fclose(in);
        return 1;
    }

    Student *students = malloc(100 * sizeof(Student));
    if (!students) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    char line[MAX_LINE_LEN];
    size_t count = 0, capacity = 100;
    while (fgets(line, sizeof(line), in)) {
        Student student;
        if (!parseLine(line, &student)) {
            fprintf(out, "Error: Invalid input format in line: %s\n", line);
            free(students);
            fclose(in);
            fclose(out);
            return 1;
        }

        if ((option == 1 && student.status != 'D') ||
            (option == 2 && student.status != 'I')) {
            continue;
        }

        if (count == capacity) {
            capacity *= 2;
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

        students[count++] = student;
    }

    fclose(in);
    mergeSort(students, 0, count - 1);

    for (size_t i = 0; i < count; i++) {
        Student *s = &students[i];
        fprintf(out, "%s %s %s-%d-%d %.3f %c", s->firstName, s->lastName, s->month, s->day, s->year, s->gpa, s->status);
        if (s->status == 'I') fprintf(out, " %d", s->toefl);
        fprintf(out, "\n");
    }

    free(students);
    fclose(out);
    return 0;
}
