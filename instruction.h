#ifndef INSTRUCTION_H
#define INSTRUCTION_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUMBER_OF_RGISTERS 8
#define MAX_LENGTH_ACTION 3


typedef struct op_code {
	char action[MAX_LENGTH_ACTION];
	int number_of_operand;
} op_code;


typedef struct instruction_command {
	char *line;
	int action;
	int target_operand;
	int source_operand;
	int lines_number_in_command;
	int line_number;
} instruction_command;

/*Searches for a register in the register array based on its name. Returns the position of the register in the array or -1 if the register is not found.*/
int search_reg(char *operand);

/*This function searches for an opcode in the OP_CODE array based on the given current word. It returns the index of the opcode in the array if found, or -1 if the opcode is not found.*/
int search_op_code(char *curr_word);

/*This function extracts the action opcode from a given line of code. 
It returns the extracted action as a string. The flag parameter indicates whether the action is the first or second word in the line.*/
char *action(char *line, int flag);

/*This function extracts the first operand from a given line of code, either before or after a comma. It returns the extracted first operand as a string.*/
char *first_operand_tow(char *line, int flag);

/*This function extracts the first operand from a given line of code, assuming the operand is the second word after the first space. It returns the extracted first operand as a string.*/
char *first_operand_one(char *line, int flag);

/*This function extracts the second operand from a given line of code, assuming the operand is the third word after the first space. It returns the extracted second operand as a string.*/
char *second_operand(char *line, int flag);

/*This function checks if a given operand contains an array sign '['.*/
int find_arr_sign(char *operand);

/*This function determines the type of the operand based on certain conditions*/
int find_operand_type(char *operand);

/*This function counts the number of words in a given line of text based on spaces and commas.*/
int word_number(char *line, int flag);

/*This function extracts from the instruction line we received the name of the operation the type of the first operand and the type of the second operand if they exist and sends the details to fill fields in the instruction data structure*/
void define_first_word(char *line, int flag, int *IC);	

/*This function checks how many lines should be added according to the form of the operand*/
int num_of_words_to_add(char *operand);

/*Building the instruction data structure and adding to its array*/
void build_operand_command(int action, int first_operand, int second_operand, int number_of_lines, char *line, int *IC);

/*This function checks whether there is a label at the beginning of the line or not*/
int with_label(char *line);

char *read_line(FILE *file);

/*This function checks if there is a label at the beginning of the line and sends the line to be processed with this data*/
void is_instruction(char *line, int *IC);

/*This function frees the data from the instruction set*/
void free_instructions();

extern instruction_command *instructions;

extern op_code OP_CODE[];

extern char *registers[];

extern int instructions_counter;

extern instruction_command *instructions;


#endif

