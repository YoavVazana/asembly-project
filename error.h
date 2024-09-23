#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Searches for errors in a line of code */
int search_error(char *line, int line_number);

/* Checks if the memory limit has been reached */
int memory_limitation();

/* Checks if a line is empty */
int empty_line(char *line);

/*Checks the type of opcode in a line and validates the operands accordingly.
This function analyzes the opcode in the line and verifies if its operands are correctly defined based on the opcode's type.*/
int op_code_type_check(char *line, int line_number);

/*Checks the type of string in a line and validates its format. This function analyzes the string in the line and verifies if it is correctly formatted.*/
int string_type_check(char *line, int line_number);

/*Checks for consecutive commas and commas at the end of a line. This function examines the line and identifies issues related to comma usage.*/
int check_comma(char *line, int line_number);

/*Checks the type of data in a line and validates its format.
This function examines the line and ensures that the data elements are correctly defined.*/
int data_type_check(char *line, int line_number);

/*Processes .extern and .entry directives in a line. This function analyzes the line for .extern and .entry directives and processes them accordingly.*/
int extentry(char *line, int line_number);

/*Checks if a token should be saved based on its type.
   This function examines a token and determines if it should be saved based on certain criteria.*/
int save_words(char *token);

/*Checks if a token should be saved for .extern and .entry processing based on its type. This function examines a token and determines if it should be saved based on certain criteria.*/
int save_words_for_entry(char *token);

/*Checks if a token is not an integer. This function checks if a token contains characters that are not part of an integer.*/
int is_not_integer(char* token);

/*Processes a line to define a constant. This function processes a line to define a constant and checks for validity.*/
int define_line(char *line, int line_number);

/*Prints an error message when no operand is found.
This function is called when no known operand is found in a line.*/
void no_operand_found();

/*Prints an error message when no hyphen sign is found in a string definition line. This function is called when no hyphen sign is found in a line that defines a string.*/
void no_hyphen_sign();

/*Prints an error message when a numeric constant is not found in the symbol table. This function is called when a numeric constant is not found in the symbol table.*/
void not_found_permanent();

/*Handles error detection in operands based on the action and operands.
This function checks for errors in operands based on the action and operands provided.*/
void error_detection_in_operands(int action, int source_operand, int target_operand);

/*Determines the type of line based on the first token. This function analyzes the first token of a line and determines the line type based on certain keywords.*/
int search_line_type(char *line);

/*Checks if a label, register, or opcode is already defined. This function checks if a label, register, or opcode is already defined in the system.*/
int double_definition(char *curr_word);

/* Prints an error message for using a label at the beginning of an entry or extern line. This function prints an error message when a label is used at the beginning of an entry or extern line.*/
void wrong_label();

/*Prints an error message for lines that exceed the maximum line length.
This function prints an error message when a line exceeds the maximum allowed length.*/
void too_long_line(char *line, int line_number);

/*Prints an error message for defining a macro twice.
This function prints an error message when attempting to define a macro twice.*/
void define_macro_twice(char *macro_name);

/* Checks if a label name is also a macro name.
This function checks if a label name matches a macro name in the system.*/
void label_mcr_name(char *label_name);

/*Checks if a word is null after memory allocation. This function checks if a word is null after memory allocation, indicating a memory allocation failure*/
int null_word(char *word);


extern int error_flag;


#endif
