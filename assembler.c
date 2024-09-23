#include "macro_pass.h"
#include "labels.h"
#include "instruction.h"
#include "error.h"
#include "directives.h"
#include "convert_to_binary.h"
#include "list.h"
#include "assembler.h"

/*This file is responsible for the central operations of the assembler*/

/*In this file there are the two functions the first_pass function and the second_pass function and they are the two passes of the assembler and in addition auxiliary functions for each of them.
The output files of the assembler, the object file, extern file and entry file are also built in this file*/


/*Reads a line from a file, removes the newline character, and returns the line.*/
char *read_line(FILE *file) {

	char *line = malloc(sizeof(char)*MAX_LINE_LENGTH);

	/*Extracts lines from the file we received*/
	if(fgets(line, MAX_LINE_LENGTH, file) == NULL) {

		return NULL;
	}

	line[strcspn(line, "\n")] = '\0';
	
	return line;
}





/*Performs the first pass of assembly on a given file.*/
int first_pass(FILE *file) {
	
	char *line;
	
	/*IC the instruction image starts at 100, DC the data image, flag indicates if there are errors*/
	int IC = START_OF_MAMMORY, DC = 0, flag = 0, line_number = 1;
	
	while((line = read_line(file)) != NULL) {
		
		/*The three functions arrange the text*/
		keep_one_space(line);
		
		remove_space_before_comma(line);
		
		remove_space_after_comma(line);
		
		/*If there were errors we will mark*/
		if(search_error(line, line_number)) {

			flag = 1;
		}
		
		/*Sending to analyze the line*/
		line_pers(line, &IC, &DC);
		
		line_number++;	
	}
	
	/*Adds to all the data we received IC*/
	fix_dc(&IC);
	
	/*Marks the lines where an entry was defined*/
	fix_entrys();

	/*Checking whether we have exceeded the allocated amount of memory*/
	if(memory_limitation()) {

		flag = 1;
	}

	/*Returns 1 if there were errors*/
	if(flag || error_flag) {

		return 1;
	}

	return 0;
}




/*Parses a line of code during the assembly process.*/

/* This function parses the given line, extracting tokens and processing them based on their type.
It handles comments, numbers, labels, directives, and op codes.*/
void line_pers(char *line, int *IC, int *DC) {

	char *token;

	int action;
	
	int check = 0;

	int directive_word;

	char *save_line;

	char *tok;

	int flag = 0;

	char *line_copy;

	save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);
	
	token = strtok(save_line, " ");
	
	while(check != 1) {
		
		/*Check for comment or end of line*/
		if(strncmp(token, ";", 1) == 0 || strncmp(token, "\n", 1) == 0) {
			
			free(save_line);

			free(line);

			return;
		}

		/*Check if line is a number definition*/
		if(is_number(token)) {
			
			label_search(line, token, 0, 0);

			free(save_line);

			free(line);

			return;
		}
			
		/*Check if token is a label*/
		if(check_if_label(token)) {
			
			/*label definition*/
			label_search(line, token, IC, DC);
						
			if(strcmp(token, ".extern") != 0) {
				
				if(strlen(token) == 2) {

					line_copy = malloc(strlen(line) + 1);

					strcpy(line_copy, line);

					tok = strtok(line_copy, " ");

					tok = strtok(NULL, " ");

					flag = 1;
					
				}
				
				token = strtok(NULL, " ");
				
			}
				
		}
		

		if(flag) {

			strcpy(token, tok);

			free(line_copy);
		}
		
		/*Search for directive word*/
		directive_word = search_directive(token);

		if(directive_word != -1) {
			
			classification_guidelines(line, directive_word, DC);
		
		}
		
		action = search_op_code(token);
		
		/*Search for op code action*/
		if(action != -1) {
			
			is_instruction(line, IC); 

		}
		check++;			
	}
	
	free(save_line);

	free(line);
}




/*Fixes entry symbols in the symbol table.*/

/*This function updates the place of entry symbols in the symbol table based on their label numbers.
It iterates through the entry symbols list, finds the corresponding label in the symbol table,
and assigns the label's number to the entry symbol's place attribute.*/
void fix_entrys() {
	
	int i,temp;
	
	/*Iterate through the entry symbols list*/
	for(i = 0; i < entrys_counter; i++) {

		/*Find the label in the symbol table*/
		temp = find_label(entrys[i].tag_name);

		/*Update the place attribute with the label's number*/
		entrys[i].place = labels[temp].label_number;
	}
}




/*Fixes data-related label numbers and directive line numbers.*/

/*This function adjusts the label numbers for labels associated with data sections and updates the line numbers for directives in the directive table by adding the given instruction counter (IC) value to their current numbers.*/
void fix_dc(int *IC) {

	int i;

	/*Update label numbers for data-related labels*/
	for(i = 0; i < label_counter; i++) {
		
		if(strcmp(labels[i].feature, "data") == 0) {

			labels[i].label_number += *IC;
		}
	}
	
	/*Update line numbers for directives*/
	for(i = 0; i < directive_counter; i++) {
		
		directives[i].line_number += *IC;
	}
}




/*Finds the instruction counter (IC) value for a given line in the assembly code.*/

/*This function searches for the instruction counter value corresponding to a specific line
in the assembly code, based on the line type provided.*/
void find_ic(char *line, int type_line, int  *IC) {
	
	int i;

	/*Check if the line is a data or string directive*/
	if(type_line == 1) {

		for(i = 0; i < directive_counter; i++) {

			if(strcmp(line, directives[i].line) == 0) {
				
				*IC = directives[i].line_number;

				IC++;

				break;
			}
		}

	/*Check if the line is an instruction*/
	}else if(type_line == 2) {

		
		for(i = 0; i < instructions_counter; i++) {
		
			if(strcmp(line, instructions[i].line) == 0) {
	
				*IC = instructions[i].line_number;

				break;
			}
		}
		
		
	}	
	
}




/*Executes the second pass of the assembly process.*/

/*This function performs the second pass of assembly by reading lines from the input file, processing them, and generating binary commands for data and code sections based on the assembly language instructions encountered.*/
void second_pass(FILE *file) {
	
	int check, save_ic, IC_value;

	int *IC = &IC_value;

	char *line;

	/*Iterate through lines in the input file*/
	while((line = read_line(file)) != NULL) {
		
		/*The three functions arrange the text*/
		keep_one_space(line);
		
		remove_space_before_comma(line);
		
		remove_space_after_comma(line);
		
		check = line_type(line);

		/*Skip processing if it's not a valid line type*/
		if(check < 1) {

			free(line);

			continue;
		}
		
		/*Find the number line command for the line*/
		find_ic(line, check, IC);
		
		save_ic = *IC;

		/*Count external symbols in the line*/
		count_externs(line, save_ic);		

		/*Build binary commands based on line type*/
		if(check == 1) {
			
			build_data_binary_comm(line, save_ic);

		} else if(check == 2) {
			
			build_code_binary_comm(line, save_ic);
		}
	
			
		free(line);
	
	}
}
	


/*Determines the type of line in the assembly code.*/

/*This function analyzes a line of assembly code to determine its type:
- If the line contains a directive(data or string), it returns 1.
- If the line contains a directive(entry or extern), it returns 0.
- If the line contains an instruction, it returns 2.
- If the line does not match any of the above criteria, it returns -1.*/
int line_type(char *line) {
	
	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);

	token = strtok(save_line, " ");

	/*Check if the first token is a data or string directive*/
	if(search_directive(token) == 0 || search_directive(token) == 1) {

		free(save_line);

		return 1;

	/*Check if the first token is a entry or extern directive*/
	}else if(search_directive(token) == 2 || search_directive(token) == 3 || strcmp(token,".define") == 0) {
		
		free(save_line);

		return 0;

	/*Check if the first token is an instruction*/
	}else if(search_op_code(token) != -1) {

		free(save_line);

		return 2;
	}

	token = strtok(NULL, " ");

	/*Check if the second token is a data or string directive*/
	if(search_directive(token) == 0 || search_directive(token) == 1) {

		free(save_line);

		return 1;

	/*Check if the second token is a entry or extern directive*/
	}else if(search_directive(token) == 2 || search_directive(token) == 3) {
		
		free(save_line);

		return 0;

	/*Check if the second token is an instruction*/
	}else if(search_op_code(token) != -1) {

		free(save_line);
		
		return 2;

	} else {

		/*If no match is found, return -1*/
		return -1;
	}

	free(save_line);

	return 0;
}
	



/*Checks if a given token (string) represents a directive to skip in assembly code processing.*/
int skip_line(char *token) {
	
	if((strcmp(token, ".extern") == 0) || (strcmp(token, ".data") == 0) ||(strcmp(token, ".string") == 0)) {
		
		return 0;
	}
	
	return 1;
}
	


/*Counts external symbols and updates their place values.*/

/*This function counts external symbols in a line of assembly code and updates their place values with the provided instruction counter (IC) value.*/
void count_externs(char *line, int IC) {

	char *token;

	int i;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);

	token = strtok(save_line, " ");

	/*Skip processing if the line should be skipped*/
	if(!skip_line(token)) {

		free(save_line);
		
		return;
	}
	
	
	/*Skip processing if the line should be skipped*/
	if(check_if_label(token)) {

		token = strtok(NULL, " ");

		if(!skip_line(token)) {

			free(save_line);
			
			return;
		}
	}

	token = strtok(NULL, ",");

	while(token != NULL) {
		
		for(i = 0; i < externs_counter; i++) {

			if(strcmp(token, externs[i].tag_name) == 0) {
				
				externs[i].place[externs[i].counter] = IC;
				
				externs[i].counter++;
	
			}
			
		}
		
		token = strtok(NULL, ",");
	}

	free(save_line);

}




/*Generates the object file based on the assembled program.*/

 /* This function produces the object file, containing information about the assembled program, including the total number of instructions and directives, as well as the encoded commands.*/
void production_ob_file(FILE *file_ob) {

	int directives_sum;

	int instructions_sum;

	char new_line[NEW_LINE_SIZE];

	char *new_encoded_command;

	Node *current;

	directives_sum = count_directives();

	instructions_sum = count_instructions();

	sprintf(new_line, "   %d  %d   \n", instructions_sum, directives_sum);

	fprintf(file_ob, "%s", new_line);

	current = head;

	while(current != NULL) {

		/*Convert the binary command to base four encoding*/
		new_encoded_command = convert_binary_to_base_four(current->data.string);

		/* Format and write the line to the object file*/
		sprintf(new_line, " %d   %s\n", current->data.line_number, new_encoded_command);

		fprintf(file_ob, "%s", new_line);

		current = current->next;

		free(new_encoded_command);
	}
}
	
	

/*Converts a binary command to base four encoding.*/

/*This function converts a binary command into base four encoding according to specific rules: 0 becomes '*', 1 becomes '#', 2 becomes '%', and 3 becomes '!'.*/
char* convert_binary_to_base_four(char *command) {

	char *base_four_command;

	int i,j;

	char digit1, digit2;

	base_four_command = malloc(sizeof(char) * BASE_FOUR_SIZE);

	for(i = 0, j = 0; i < BINARY_WORD_LENGTH - 1; i += 2, j++) {
		
		digit1 = command[i];

		digit2 = command[i + 1];

		/*Convert each pair of binary digits to base four character*/
		if(digit1 == '0' && digit2 == '0') {

			base_four_command[j] = '*';

		} else if(digit1 == '0' && digit2 == '1') {

			base_four_command[j] = '#';

		} else if(digit1 == '1' && digit2 == '0') {

			base_four_command[j] = '%';

		} else if(digit1 == '1' && digit2 == '1') {

			base_four_command[j] = '!';
		}
	}

	/*Add null terminator to the base four command string*/
	base_four_command[BASE_FOUR_SIZE - 1] = '\0';

	return base_four_command;

}


/*Builds the external symbols file.*/

 /* This function constructs the external symbols file, including the external symbols' names and their corresponding positions in the program.*/
void build_ext_file(FILE *file_ext) {

	char new_line[MAX_LINE_LENGTH];

	int i, j;

	for(i = 0; i < externs_counter; i++) {

		for(j = 0; j < externs[i].counter; j++) {

			/*Format and write each external symbol entry to the file*/
			sprintf(new_line, "%s  %d\n",externs[i].tag_name, externs[i].place[j]);

			fprintf(file_ext, "%s", new_line);

		}
	}
}



/*Builds the entry symbols file.*/

/*This function constructs the entry symbols file, including the entry symbols' names and their corresponding positions in the program.*/
void build_ent_file(FILE *file_ent) {

	char new_line[MAX_LINE_LENGTH];

	int i;

	for(i = 0; i < entrys_counter; i++) {

		/*Format and write each entry symbol entry to the file*/
		sprintf(new_line, "%s  %d\n", entrys[i].tag_name, entrys[i].place);

		fprintf(file_ent, "%s", new_line);

	}
}
		



/*Counts the total number of instructions in the program.*/
 
/* This function calculates the total number of instructions by summing up the lines contributed by each instruction command.*/
int count_instructions() {

	int i;

	int counter = 0;

	for(i = 0; i < instructions_counter; i++) {

		counter += instructions[i].lines_number_in_command;
	}

	return counter;
}



/*Counts the total number of directives in the program.*/

/*This function calculates the total number of directives by summing up the lines contributed by each directive command.*/
int count_directives() {

	int i;

	int counter = 0;

	for(i = 0; i < directive_counter; i++) {

		counter += directives[i].add_lines;
	}

	return counter;
}



