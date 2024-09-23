#include "convert_to_binary.h"
#include "directives.h"
#include "instruction.h"
#include "labels.h"
#include "error.h"
#include "list.h"

/*Converts an integer number to a binary representation.*/
char *int_to_binary(int num, int label_num) {

	int i;

	char *are;

	char* full_binary_command;

	char binary_number[BINARY_NUMBER];

	full_binary_command = (char*) malloc(sizeof(char) * BINARY_WORD_LENGTH);

	/*Creates the conversion*/
	for(i = BINARY_NUMBER - 2; i >= 0; i--) { 
		
		binary_number[i] = (num & 1) + '0';

		num >>= 1;
		
		
	}

	binary_number[BINARY_NUMBER - 1] = '\0';

	/* Determine ARE value */
	if(label_num == -1) {
		
		are = operand_to_binary(0);
	} else {

		are = operand_to_binary(labels[label_num].are);
	}
	
	/* Combine binary number and ARE value */
	sprintf(full_binary_command, "%s%s", binary_number, are);
	
	full_binary_command[BINARY_WORD_LENGTH - 1] = '\0';

	free(are);

	

	return full_binary_command;
}



/* Converts an integer number to a binary representation for data */
char *int_to_binary_for_data(int num) {

	int i;

	char* binary_number = (char*) malloc(sizeof(char) * BINARY_WORD_LENGTH);

	 /* Create the binary conversion */
	for(i = BINARY_WORD_LENGTH - 2; i >= 0; i--) { 

		binary_number[i] = (num & 1) + '0';

		num >>= 1;
	}

	binary_number[BINARY_WORD_LENGTH - 1] = '\0';

	return binary_number;
}



/* Converts register numbers to binary representation */
char *reg_to_binary(char *word, int reg1, int reg2) {
	
	char *reg_source;

	char *reg_target;

	reg_source = reg_num_to_binary(reg1);

	reg_target = reg_num_to_binary(reg2);

	/* Format binary representation for registers */
	sprintf(word, "000000%s%s00", reg_target, reg_source);

	free(reg_source);

	free(reg_target);

	return word;
}



/* Converts a register number to its binary representation */
char *reg_num_to_binary(int num) {
	int i;
	char* reg_binary =(char*) malloc(sizeof(char) * BINARY_REG_SIZE);

	/* Create the binary conversion */
	for(i = BINARY_REG_SIZE - 2; i >= 0; i--) { 

		reg_binary[i] = (num & 1) + '0';
		
		num >>= 1;
	}
	
	reg_binary[BINARY_REG_SIZE - 1] = '\0';
	return reg_binary;
}


/* Converts an action number to its binary representation */
char *action_to_binary(int num) {
	int i;
	char* action_binary =(char*) malloc(sizeof(char) * BINARY_ACTION_SIZE);

	/* Create the binary conversion */
	for(i = BINARY_ACTION_SIZE - 2; i >= 0; i--) { 

		action_binary[i] = (num & 1) + '0';
		
		num >>= 1;
	}
	
	action_binary[BINARY_ACTION_SIZE - 1] = '\0';
	return action_binary;
}



/* Converts an operand number to its binary representation */
char *operand_to_binary(int num) {
	int i;
	char* operand_binary =(char*) malloc(sizeof(char) * BINARY_OPERAND_SIZE);

	/* Create the binary conversion */
	for(i = BINARY_OPERAND_SIZE - 2; i >= 0; i--) {
		
		operand_binary[i] = (num & 1) + '0';

		num >>= 1;
	}
	
	operand_binary[BINARY_OPERAND_SIZE - 1] = '\0';
	return operand_binary;
}



/* Builds the first binary command based on action, source, and target */
void build_first_binary_command(int action, int source, int target, char* first_command) {

	char *action_binary;

	char *source_binary;

	char *target_binary;

	/* Convert action, source, and target to binary */
	action_binary = action_to_binary(action);

	source_binary = operand_to_binary(source);

	target_binary = operand_to_binary(target);

	/* Format the first binary command */
	sprintf(first_command, "0000%s%s%s00", action_binary, source_binary, target_binary);

	free(action_binary);

	free(source_binary);

	free(target_binary);

}



/* Converts an instruction with one operand to its binary representation */
void one_operand_to_binary(char *line, int i, char *first_word, int IC) {
	
	char *token;

	char second_word[BINARY_WORD_LENGTH];

	char third_word[BINARY_WORD_LENGTH];

	int reg;

	token = strtok(line, " ");

	if(check_if_label(token)) {

		token = strtok(NULL, " ");
	}

	token = strtok(NULL, " ");

	/* Determine the type of operand and convert it to binary */
	if(instructions[i].target_operand == 0) {
		
		/*Zero-type operand conversion*/
		strcpy(second_word, zero_type(second_word, token));
		
	} else if(instructions[i].target_operand == 1) {

		/*One-type operand conversion*/
		strcpy(second_word, one_type(second_word, token));

	}else if(instructions[i].target_operand == 2) {
		
		/*Type-A two-type operand conversion, Type-B two-type operand conversion*/
		strcpy(second_word, tow_type_A(second_word, token));
		
		strcpy(third_word, tow_type_B(third_word, token));
		
	}else if(instructions[i].target_operand == 3) {
		
		/*Register operand conversion*/
		reg = search_reg(token);

		reg_to_binary(second_word, 0, reg);

	}

	/*Build the binary command*/
	build_new_data_command(first_word, second_word, third_word, NULL, NULL, IC, i);

}



/* Converts an instruction with two operands to its binary representation */
void tow_operand_to_binary(char *line, int i, char *first_word, int IC) {

	char second_word[BINARY_WORD_LENGTH];

	char third_word[BINARY_WORD_LENGTH];

	char fourth_word[BINARY_WORD_LENGTH];

	char fifth_word[BINARY_WORD_LENGTH];

	char save_arg1[MAX_LINE_LENGTH];

	char save_arg2[MAX_LINE_LENGTH];

	int flag = 0;

	int flag2 = 0;

	int result, counter;

	int reg1 = 0, reg2 = 0;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	counter = word_number(save_line, 0);

	if(counter == 3) {
		result = sscanf(save_line, "%*s %[^,],%[^,]", save_arg1, save_arg2);
	
	} else {
		result = sscanf(save_line, " %*s %*s %[^,],%[^,]", save_arg1, save_arg2);
	}	

	/* Determine the type of operands and convert them to binary */

	/*Zero-type source operand conversion*/
	if(instructions[i].source_operand == 0) {

		strcpy(second_word, zero_type(second_word, save_arg1));

	/*One-type source operand conversion*/
	} else if(instructions[i].source_operand == 1) {

		strcpy(second_word, one_type(second_word, save_arg1));

	/*Type-A two-type source operand conversion, Type-B two-type source operand conversion*/
	}else if(instructions[i].source_operand == 2) {
		
		strcpy(second_word, tow_type_A(second_word, save_arg1));
		
		strcpy(third_word, tow_type_B(third_word, save_arg1));

		flag = 1;
	
	/*Register source operand conversion*/
	}else if(instructions[i].source_operand == 3) {
		
		reg1 = search_reg(save_arg1);

		if(search_reg(save_arg2) == -1) {
			
			strcpy(second_word, reg_to_binary(second_word, reg2, reg1));
		} else {
		
			flag2 = 1;
		}
	}
			
	
	/* Determine the type of target operand and convert it to binary */
	if(flag) {

		/*Zero-type target operand conversion*/
		if(instructions[i].target_operand == 0) {
		
			strcpy(fourth_word, zero_type(fourth_word, save_arg2));
		
		/*One-type target operand conversion*/
		} else if(instructions[i].target_operand == 1) {

			strcpy(fourth_word, one_type(fourth_word, save_arg2));

		/*Type-A two-type target operand conversion, Type-B two-type target operand conversion*/
		}else if(instructions[i].target_operand == 2) {
		
			strcpy(fourth_word, tow_type_A(fourth_word, save_arg2));
		
			strcpy(fifth_word, tow_type_B(fifth_word, save_arg2));
		
		/*Register target operand conversion*/
		}else if(instructions[i].target_operand == 3) {
		
			reg2 = search_reg(save_arg2);

			strcpy(fourth_word, reg_to_binary(fourth_word, reg2, reg1));

		}

	} else {

		/*Zero-type target operand conversion*/
		if(instructions[i].target_operand == 0) {
		
			strcpy(third_word, zero_type(fourth_word, save_arg2));
		
		/*One-type target operand conversion*/
		} else if(instructions[i].target_operand == 1) {

			strcpy(third_word, one_type(fourth_word, save_arg2));

		/*Type-A two-type target operand conversion, Type-B two-type target operand conversion*/
		}else if(instructions[i].target_operand == 2) {
		
			strcpy(third_word, tow_type_A(fourth_word, save_arg2));
		
			strcpy(fourth_word, tow_type_B(fifth_word, save_arg2));
		
		/*Register source operand conversion, Register target operand conversion*/
		}else if(instructions[i].target_operand == 3) {
		
			reg2 = search_reg(save_arg2);

			if(flag2) {

				strcpy(second_word, reg_to_binary(second_word, reg2, reg1));
			} else {
			
				strcpy(fourth_word, reg_to_binary(fourth_word, reg2, reg1));
			}
			
		}	
	}

	/*Build the binary command*/
	build_new_data_command(first_word, second_word, third_word, fourth_word, fifth_word, IC, i);

	free(save_line);
	
}



/* Converts a token in Type-B two-type operand to binary */
char* tow_type_B(char *word, char *token) {
	int label_num;

	char *tok_token;

	char *save_token = malloc(strlen(token + 1));
	
	/*Extract the token between square brackets*/
	strcpy(save_token, token);

	tok_token = strtok(save_token, "[");

	tok_token = strtok(NULL, "]");
	
	/*Check if the token is a label or a number and convert it to binary accordingly*/
	if(atoi(tok_token) == 0) {

		label_num = find_label(tok_token);

		/*Convert label number to binary*/
		word = int_to_binary(labels[label_num].label_number, label_num);

	} else {
		/*Convert number to binary*/
		word = int_to_binary(atoi(tok_token), -1);
	}

	free(save_token);

	return word;
	
}



/* Converts a token in Type-A two-type operand to binary */
char* tow_type_A(char *word, char *token) {
	char *tok_token;

	int label_num;

	char *save_token = malloc(strlen(token + 1));

	strcpy(save_token, token);

	/*Extract the token before square brackets*/
	tok_token = strtok(save_token, "[");
	
	/*Find the label number and convert it to binary*/
	label_num = find_label(tok_token);
	
	word = int_to_binary(labels[label_num].label_number, label_num);

	free(save_token);

	return word;
}
	
	

/* Converts a token in one-type operand to binary */
char* one_type(char *word, char *token) {

	int label_num;

	/*Find the label number and convert it to binary*/
	label_num = find_label(token);
		
	word = int_to_binary(labels[label_num].label_number, label_num);

	return word;
}



/* Converts a token in zero-type operand to binary */
char* zero_type(char *word, char *token) {

	int label_num;
	
	/*Check if the token is a label or a number and convert it to binary accordingly*/
	if(atoi(token + 1) == 0) {

		label_num = find_label(token + 1);

		/*Convert label number to binary*/
		word = int_to_binary(labels[label_num].label_number, label_num);

	} else {
		
		/*Convert number to binary*/
		word = int_to_binary(atoi(token + 1), -1);

	}

	return word;
}

	

/* Counts the number of commas in a string */
int count_comma(char *numbers) {

	int i;

	int counter = 0;

	/*Loop through the string to count commas*/
	for(i = 0; i < strlen(numbers); i++) {
		
		if(numbers[i] == ',') {

			counter++;
		}
	}
	return counter;
}
	






