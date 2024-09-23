#ifndef LABELS_H
#define LABELS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define FILE_NAME_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define MAX_LENGTH_ACTION 3
#define MAX_LABELS 100
#define START_OF_MAMMORY 100



/* Checks if a string represents a number directive (.define) */
int is_number(char *curr_word);

/* Checks if a line contains a label and determines its type */
int is_label(char *line);

/* Parses a line to extract a label's name and value from a .define directive */
void label_number_define(char *line, char *name, int *value);

/* Extracts a label's name from a line */
void label_name_define(char *line, char *name);

/* Builds the label table with the specified label name, number, feature, and ARE type */
void build_table_label(char *label_name, int *label_number, int feature, int labels_size);

/* Finds the index of a label in the label table based on its name */
int find_label(char *curr_word);

/* Searches for labels and processes label definitions in the input line */
void label_search(char *line, char *curr_word, int *IC, int *DC);

/* Frees allocated memory for label names and the label table */
void free_labels();

/* Processes an .extern directive line, adding the external label to the label table */
void extern_line(char *line);



typedef struct label {

	char *tag_name;

	int label_number;

	char feature[7];

	int are;
} label;

extern label *labels;

extern int label_counter;

#endif
