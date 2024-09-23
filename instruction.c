#include "list.h"
#include "instruction.h"
#include "labels.h"
#include "macro_pass.h"
#include "error.h"


/* General Explanation for the Following Functions:
   - instructions_counter is used to count the number of instructions created.
   - instructions is an array that stores the details of the instructions created.
   - OP_CODE is an array defining the names of instructions and their operand counts.*/

int instructions_counter = 0;

instruction_command *instructions = NULL;

op_code OP_CODE[] = {
	{"mov", 2},
	{"cmp", 2},
	{"add", 2},
	{"sub", 2},
	{"not", 1},
	{"clr", 1},
	{"lea", 2},
	{"inc", 1},
	{"dec", 1},
	{"jmp", 1},
	{"bne", 1},
	{"red", 1},
	{"prn", 1},
	{"jsr", 1},
	{"rts", 0},
	{"hlt", 0}
};

char *registers[] = {"r0","r1", "r2", "r3", "r4", "r5", "r6", "r7"};



/*Searches for a register in the register array based on its name. Returns the position of the register in the array or -1 if the register is not found.*/
int search_reg(char *operand) {

	int i;

	for(i = 0; i < NUMBER_OF_RGISTERS; i++) {
		
		if(strcmp(operand, registers[i]) == 0) {
			

			return i;

		}
	}

	return -1;
}



/*This function searches for an opcode in the OP_CODE array based on the given current word.
 It returns the index of the opcode in the array if found, or -1 if the opcode is not found.*/
int search_op_code(char *curr_word) {

	int i;
	
	for(i = 0; i < 16; i++) {
		
		if(strcmp(OP_CODE[i].action, curr_word) == 0) {
			
			return i;
		}
	}

	return -1;
}



/*This function extracts the action opcode from a given line of code. 
It returns the extracted action as a string.
The flag parameter indicates whether the action is the first or second word in the line.*/
char *action(char *line, int flag) {

	char *action;

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	if(!flag) {

		action = malloc(sizeof(char) * strlen(token));

		strcpy(action, token);

		free(save_line);

		return action;

	} else {

		token = strtok(NULL, " ");

		action = malloc(sizeof(char) * strlen(token));

		strcpy(action, token);

		free(save_line);

		return action;

	}
}




/*This function extracts the first operand from a given line of code, either before or after a comma.
It returns the extracted first operand as a string.
The flag parameter determines whether the function extracts the first operand before (flag = 0) or after (flag = 1) a comma.
If flag is 0, the function extracts the operand immediately after the first space in the line.
If flag is 1, the function extracts the operand after the second space in the line, assuming a comma separates them.*/
char *first_operand_tow(char *line, int flag) {

	char *first_operand;

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");
	
	if(!flag) { 

		token = strtok(NULL, ",");

		first_operand = malloc(sizeof(char) * strlen(token));

		strcpy(first_operand, token);

		free(save_line);

		return first_operand;

	} else {

		token = strtok(NULL, " ");

		token = strtok(NULL, ",");

		first_operand = malloc(sizeof(char) * strlen(token));

		strcpy(first_operand, token);

		free(save_line);

		return first_operand;
	}
}



/*This function extracts the first operand from a given line of code, assuming the operand is the second word after the first space.
It returns the extracted first operand as a string.
The flag parameter determines whether the function extracts the first operand with or without considering additional words.
If flag is 0, the function extracts only the first word after the first space in the line as the operand.
If flag is 1, the function extracts the second word after the first space in the line as the operand.
*/
char *first_operand_one(char *line, int flag) {

	char *first_operand;

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	if(!flag) {

		token = strtok(NULL, " ");

		first_operand = malloc(sizeof(char) * strlen(token));

		strcpy(first_operand, token);

		free(save_line);

		return first_operand;

	} else {

		token = strtok(NULL, " ");

		token = strtok(NULL, " ");	

		first_operand = malloc(sizeof(char) * strlen(token));

		strcpy(first_operand, token);

		free(save_line);

		return first_operand;
	}
}


/*This function extracts the second operand from a given line of code, assuming the operand is the third word after the first space.
It returns the extracted second operand as a string.
The flag parameter determines whether the function extracts the second operand with or without considering additional words.
If flag is 0, the function extracts only the second word after the first space in the line as the operand.
If flag is 1, the function extracts the third word after the first space in the line as the operand.*/
char *second_operand(char *line, int flag) { 

	char *second_operand;

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");
	
	if(!flag) {

		token = strtok(NULL, ",");

		token = strtok(NULL, " ");

		second_operand = malloc(sizeof(char) * strlen(token));

		strcpy(second_operand, token);

		free(save_line);

		return second_operand;

	} else {

		token = strtok(NULL, " ");

		token = strtok(NULL, ",");

		token = strtok(NULL, " ");

		second_operand = malloc(sizeof(char) * strlen(token));

		strcpy(second_operand, token);

		free(save_line);

		return second_operand;
	}
}



/* This function checks if a given operand contains an array sign '['.
If the array sign is found, the function returns 1; otherwise, it returns 0.
*/
int find_arr_sign(char *operand) {

	int i;

	for(i = 0; i < strlen(operand); i++) {

		if(operand[i] == '[') {
			
			return 1;
		}
	}
	
	return 0;
}



/*This function checks if a given operand starts with an alphabetic character, indicating it is a label.
If the first character of the operand is alphabetic, the function returns 1; otherwise, it returns 0.
*/
int its_label(char *operand) {
	
	if(isalpha(operand[0])) {
		
		return 1;
	}
	return 0;
}



/*This function determines the type of the operand based on certain conditions:
   - If the operand starts with '#', it is an immediate operand (return 0).
   - If the operand is a label, not an array, and not a register, it is a direct operand (return 1).
   - If the operand contains an array sign '[', it is an array operand (return 2).
   - If the operand is a register, it is a register operand (return 3).
   - If none of the above conditions are met, the function returns -1, indicating an invalid operand type.*/
int find_operand_type(char *operand) {
	
	if(operand[0] == '#') {

		return 0;

	} else if(its_label(operand) && !find_arr_sign(operand) && search_reg(operand) == -1) {

		return 1;

	}else if(find_arr_sign(operand)) {
		
		return 2;

	}else if(search_reg(operand)) {

		return 3;

	} else {

		return -1;
	}
}



/*This function counts the number of words in a given line of text based on spaces and commas.*/
int word_number(char *line, int flag) {
	
	int i, counter = 1;

	for(i = 0; line[i] != '\0'; i++) {

		if(line[i] == ' ' || line[i] == ',') {

			counter++;
		}
	}
	
	if(flag) {

		counter--;
		return counter;

	} else {
		return counter;
	}
}
		

/*This function extracts from the instruction line we received the name of the operation the type of the first operand and the type of the second operand if they exist and sends the details to fill fields in the instruction data structure*/		
void define_first_word(char *line, int flag, int *IC) {
	
	char *action_word;

	char *first_operand;

	char *second_operand_word;

	int action_val;

	int first_operand_val;

	int second_operand_val;
	
	int numbers_of_words = word_number(line, flag);

	int number_of_lines;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line); 
	
	if(numbers_of_words == 1) {

		action_word = action(line, flag);
		
		action_val = search_op_code(action_word);

		first_operand_val = 0;

		second_operand_val = 0;

		number_of_lines = 1;
	
	} else if(numbers_of_words == 2) {

		action_word = action(line, flag);
		
		action_val = search_op_code(action_word);
		
		first_operand = first_operand_one(line, flag);

		first_operand_val = 0;

		second_operand_val = find_operand_type(first_operand);

		number_of_lines = 1 + num_of_words_to_add(first_operand);
		
	}else if(numbers_of_words == 3) {

		action_word = action(line, flag);

		action_val = search_op_code(action_word);

		first_operand = first_operand_tow(line, flag);

		first_operand_val = find_operand_type(first_operand);

		second_operand_word = second_operand(line, flag);

		second_operand_val = find_operand_type(second_operand_word);

		number_of_lines = 1 + num_of_words_to_add(first_operand) + num_of_words_to_add(second_operand_word);
		
		
		if(find_operand_type(first_operand) == 3 && find_operand_type(second_operand_word) == 3) {
			number_of_lines--;
		}

	}

	free(save_line);

	error_detection_in_operands(action_val, first_operand_val, second_operand_val);
	
	build_operand_command(action_val, first_operand_val, second_operand_val, number_of_lines, line, IC);
	

}



/*This function checks how many lines should be added according to the form of the operand*/
int num_of_words_to_add(char *operand) {

	int counter = 0;

	if(find_operand_type(operand) == 0) {

		counter++;

	} else if(find_operand_type(operand) == 1) {

		counter++;

	} else if(find_operand_type(operand) == 2) {

		counter += 2;

	} else if(find_operand_type(operand) == 3) {

		counter++;

	} else {

		no_operand_found();

		
	}

	return counter;
}
	

/*Building the instruction data structure and adding to its array*/
void build_operand_command(int action, int first_operand, int second_operand, int number_of_lines, char *line, int *IC) {

	instructions = realloc(instructions, sizeof(instruction_command) * (instructions_counter + 1));

	instructions[instructions_counter].line = malloc(sizeof(char) * (strlen(line)+1));

	strcpy(instructions[instructions_counter].line,line);
	
	instructions[instructions_counter].action = action;

	instructions[instructions_counter].source_operand = first_operand;

	instructions[instructions_counter].target_operand = second_operand;

	instructions[instructions_counter].lines_number_in_command = number_of_lines;

	instructions[instructions_counter].line_number = *IC;
	*IC += number_of_lines;
	instructions_counter++;
}



/*This function checks whether there is a label at the beginning of the line or not*/
int with_label(char *line) {

	char *token = strtok(line, " ");

	if(search_op_code(token) != -1) {

		return 0;
	}

	token = strtok(NULL, " ");

	if(search_op_code(token) != -1) {

		return 1;
	}
	
	return -1;
}



/*This function checks if there is a label at the beginning of the line and sends the line to be processed with this data*/
void is_instruction(char *line, int *IC) {

	char *save_line;

	int flag;
	
	save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);
	
	flag = with_label(save_line);
		
	define_first_word(line, flag, IC);
		
	free(save_line);
		
}


/*This function frees the data from the instruction set*/
void free_instructions() {

	int j;

	for(j = 0; j < instructions_counter; j++) {
		
		free(instructions[j].line);
	}

	free(instructions);
}


