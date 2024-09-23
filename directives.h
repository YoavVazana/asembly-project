#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ENTRY_LINES 100
#define MAX_LINE_LENGTH 81
#define MAX_EXTERN_LINES 100
#define FILE_NAME_LENGTH 80
#define GUIDELINES_ARR_LENGTH 4

char *read_line(FILE *file);

/*Builds an entry command structure.*/
void build_entry_command(char *line, int *place);

/*Builds an extern command structure.*/
void build_extern_command(char *line, int *place);

/*This function counts the number of elements separated by commas in the given line.*/
int count_numbers(char *line);

/*This function searches for a string enclosed in double quotes within the given line and returns the extracted string.*/
char *find_string(char *line);

/*This function extracts numbers (including negative numbers) and separators (comma) from the given line and returns them as a string.*/
char *find_numbers(char *line);	

/*This function modifies a field in a define directive line according to the label number associated with the field if it represents a label. It replaces label names with their corresponding label numbers.*/
char *change_define_field(char *numbers);

/*This function processes a data instruction line, extracts the numbers or strings from it, and builds a command structure to represent the data instruction, and from there the function updates the instruction table with the new command.*/
int build_data_command(char *line, int data_type, int *DC);

/*This function takes a directive line, its corresponding index, and the current
data counter (`DC`), and processes the line according to the directive guideline index.*/
void classification_guidelines(char *line, int i, int *DC);

/*This function searches for a given directive keyword (`curr_word`) in the guidelines array. If the keyword is found, it returns the index of the directive in the guidelines array.*/
int search_directive(char *curr_word);

/*This function checks if the given word is a label (ends with ':') or an extern directive.*/
int check_if_label(char *curr_word);

/*This function frees the memory allocated for directive structures such as entry commands, extern commands, and directive commands. It iterates through the arrays holding these structures and frees the memory for each element.*/
void free_directives();



typedef struct directive_command {

	char *line;

	int command[MAX_LINE_LENGTH];

	int add_lines;

	int line_number;

} directive_command;



typedef struct entry_command {

	char *tag_name;

	int place;

} entry_command;



typedef struct extern_command {

	char *tag_name;

	int place[MAX_EXTERN_LINES];

	int counter;

} extern_command;


extern entry_command *entrys;

extern extern_command *externs;

extern int directive_counter;

extern int entrys_counter;

extern int externs_counter;

extern directive_command *directives;

#endif
