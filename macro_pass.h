#ifndef MACRO_PASS_H
#define MACRO_PASS_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 81
#define MAX_MACROS 100
#define MAX_MACRO_NAME 80
#define MAX_MAC_DEF 1000


typedef struct macro {
	
	char name[MAX_LINE_LENGTH];

	char *definition;

	int macro_length;

} macro;

extern int macro_count;

extern macro *macros;

/*This function removes spaces before commas in a given text.*/
void remove_space_before_comma(char *text);

/*This function removes spaces after commas in a given text.*/
void remove_space_after_comma(char *text);

/*This function keeps only one space between words in a given text.*/
void keep_one_space(char *text);

/*Expands macros in a given input file and writes the expanded output to another file.*/
void expand_macros(FILE *file_in, FILE *file_out);

/*Adds a macro definition to the macros array.*/
void add_macro(char *name, char *mac_definition, int macro_size);

/*Extracts the macro name from a line containing a macro definition.*/
void mac_name(char *line, char *macro_name);

/*Reads a line from a file and performs preprocessing for macros. This function removes leading and trailing spaces, and adjusts spaces around commas.*/
char *read_line_for_macro(FILE *file);

/*Checks if a line contains a macro definition.*/
int is_macro(char *line);

/*Finds a macro by its name in the macros array.*/
int find_macro(char *name);

/*Replaces a macro name with its definition in a given line.*/
char *replace_macro(char *line);
	
/*This function frees the memory allocated for macros and their definitions.*/
void free_macros();

#endif

