#ifndef CONVERT_TO_BINARY_H
#define CONVERT_TO_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BITS 15
#define BINARY_REG_SIZE 4
#define BINARY_ACTION_SIZE 5
#define BINARY_OPERAND_SIZE 3
#define BINARY_NUMBER 13
#define BINARY_WORD_LENGTH 15
#define MAX_WORDS 4095

/*Converts an integer number to a binary representation.*/
char *int_to_binary(int num, int label_num);

/* Converts an integer number to a binary representation for data */
char *int_to_binary_for_data(int num); 

/* Converts register numbers to binary representation */
char *reg_to_binary(char *word, int reg1, int reg2);

/* Converts a register number to its binary representation */
char *reg_num_to_binary(int num);

/* Converts an action number to its binary representation */
char *action_to_binary(int num);

/* Converts an operand number to its binary representation */
char *operand_to_binary(int num);

/* Builds the first binary command based on action, source, and target */
void build_first_binary_command(int action, int source, int target, char* first_command);

/* Converts an instruction with one operand to its binary representation */
void one_operand_to_binary(char *line, int i, char *first_word, int IC);

/* Converts an instruction with two operands to its binary representation */
void tow_operand_to_binary(char *line, int i, char *first_word, int IC);

/* Converts a token in Type-B two-type operand to binary */
char* tow_type_B(char *word, char *token);

/* Converts a token in Type-A two-type operand to binary */
char* tow_type_A(char *word, char *token);

/* Converts a token in one-type operand to binary */
char* one_type(char *word, char *token);

/* Converts a token in zero-type operand to binary */
char* zero_type(char *word, char *token); 

/* Counts the number of commas in a string */
int count_comma(char *numbers);


#endif
