#include "error.h"
#include "macro_pass.h"
#include "labels.h"
#include "instruction.h" 
#include "list.h"
#include "directives.h"

int error_flag = 0;


/* Searches for errors in a line of code */
int search_error(char *line, int line_number) {

	int check = 0;
	
	/*Check for line length*/
	too_long_line(line, line_number);

	if(empty_line(line)) {

		/*Empty line, no error*/
		return 0;
	}


	/*Search for errors based on line type*/	
	if(search_line_type(line) == -1) {
		
		printf("line %d has no meaning in our language\n", line_number);

		return 1;

	} else if(search_line_type(line) == 0) {
		
		check = define_line(line, line_number);

	} else if(search_line_type(line) == 1) {

		check = extentry(line, line_number);

	} else if(search_line_type(line) == 2) {

		check = data_type_check(line, line_number);

	} else if(search_line_type(line) == 3) {

		check = string_type_check(line, line_number);

	} else if(search_line_type(line) == 4) {

		check = op_code_type_check(line, line_number);
	}

	if(check) {
		
		/*Error found*/
		return 1;
	} else {

		/*No error*/
		return 0;
	}
}



/* Checks if a line is empty */
int empty_line(char *line) {

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	if(token[0] == ';' || strncmp(token, "\n", 1) == 0) {

		free(save_line);

		/*Empty line*/
		return 1;
	}

	free(save_line);
	
	/*Not empty line*/
	return 0;
}

		


/* Checks if the memory limit has been reached */
int memory_limitation() {

	if(directives[directive_counter].line_number > MAX_WORDS) {

		printf("Memory is over!\n");

		/*Memory limit exceeded*/
		return 1;
	}
	
	/*Memory limit not exceeded*/
	return 0;
}



/*Checks the type of opcode in a line and validates the operands accordingly.
This function analyzes the opcode in the line and verifies if its operands are correctly defined based on the opcode's type.*/
int op_code_type_check(char *line, int line_number) {

	char *token;

	int op_code_num;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	/*Check if the line starts with a label*/
	if(check_if_label(token)) {

		token = strtok(NULL, " ");
	}

	op_code_num = search_op_code(token);


	/*Handle different types of opcodes*/

	if(op_code_num >= 14 && op_code_num <= 15) {

		token = strtok(NULL, " ");

		/*Check for incorrect operands*/
		if(token != NULL) {
		
			printf("In line %d there is an incorrect instruction definition in instructions of this type there are no operands\n", line_number);

			free(save_line);

			return 1;
		}

		free(save_line);
		
		return 0;

	} else if(op_code_num >= 5 && op_code_num <= 13) {

		token = strtok(NULL, " ");

		token = strtok(NULL, " ");

		/*Check for invalid operands*/
		if(token != NULL) {

			printf("In line %d there is an invalid instruction definition in instructions of this type can only be a target operand\n", line_number);

			free(save_line);

			return 1;
		}

		free(save_line);

		return 0;

	} else if(op_code_num >= 0 && op_code_num <= 4)
		
		token = strtok(NULL, ",");

		/*Check for incorrect number of operands*/
		if(token == NULL) {

			printf("In line %d there is an incorrect instruction definition in instructions of this type there are two operands\n", line_number);

			free(save_line);

			return 1;
		}

		token = strtok(NULL, ",");

		if(token == NULL) {

			printf("In line %d there is an incorrect instruction definition in instructions of this type there are two operands\n", line_number);

			free(save_line);

			return 1;
		}

		free(save_line);

		return 0;
}



/*Checks the type of string in a line and validates its format. This function analyzes the string in the line and verifies if it is correctly formatted.*/
int string_type_check(char *line, int line_number) {

	char *token;

	int len;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	/*Check if the line starts with a label*/
	if(check_if_label(token)) {

		token = strtok(NULL, " ");
	}

	token = strtok(NULL, " ");

	len = strlen(token);

	/*Check for invalid string format*/
	if(token[0] != '"' || token[len - 1] != '"') {

		printf("In line %d an invalid string definition\n", line_number);

		free(save_line);
		
		return 1;
	}

	free(save_line);

	return 0;
}



/*Checks for consecutive commas and commas at the end of a line. This function examines the line and identifies issues related to comma usage.*/
int check_comma(char *line, int line_number) {

	int i, len = strlen(line);

	/*Check for two consecutive commas or comma at the end*/
	for(i = 0; i < len -1; i++) {
		
		if(line[i] == ',' && line[i + 1] == ',') {

			printf("In line %d there are two consecutive commas\n", line_number);
			
			return 1;
		}
	}

	if(line[len - 1] == ',') {

		printf("In line %d there is a comma after the last figure\n", line_number);

		return 1;
	}

	return 0;
}
			


/*Checks the type of data in a line and validates its format.
This function examines the line and ensures that the data elements are correctly defined.*/
int data_type_check(char *line, int line_number) {

	int check = 0;

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);
	
	/*Check for consecutive commas or comma at the end*/
	check = check_comma(save_line, line_number);

	if(check) {

		free(save_line);
	
		return 1;
	}

	token = strtok(save_line, " ");

	/*Skip label if present*/
	if(check_if_label(token)) {

		token = strtok(NULL, " ");
	}

	token = strtok(NULL, ",");

	/*Validate each token in the line*/
	while(token != NULL) {

		if(is_not_integer(token) && find_label(token) == -1) {
			
			printf("In line %d, an attempt to insert an illegal element into the array\n", line_number);

			free(save_line);

			return 1;
		}
	
		token = strtok(NULL, ",");
		
	}

	free(save_line);

	return 0;
}




/*Processes .extern and .entry directives in a line.
This function analyzes the line for .extern and .entry directives and processes them accordingly.*/
int extentry(char *line, int line_number) {

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	token = strtok(NULL, " ");

	if(save_words_for_entry(token)) {

		token = strtok(NULL, " ");

		if(token != NULL) {

			printf("In line %d the definition is valid but there is an additional text at the end of the line\n", line_number);
		}

		free(save_line);
		
		return 0;
	
	} else {

		printf("In line %d the name is a name saved in the system\n", line_number);

		free(save_line);

		return 1;
	}
}



/*Checks if a token should be saved based on its type.
   This function examines a token and determines if it should be saved based on certain criteria.*/
int save_words(char *token) {
	
	if(search_op_code(token) != -1 || search_reg(token) != -1 || find_label(token) != -1 || strcmp(token, ".define") == 0 || strcmp(token, ".extern") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {

		/*Do not save certain token*/
		return 0;
	}
	
	/*Save other tokens*/
	return 1;
}
	


/*Checks if a token should be saved for .extern and .entry processing based on its type.
This function examines a token and determines if it should be saved based on certain criteria.*/
int save_words_for_entry(char *token) {
	
	if(search_op_code(token) != -1 || search_reg(token) != -1 || strcmp(token, ".define") == 0 || strcmp(token, ".extern") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {

		/*Do not save certain tokens*/
		return 0;
	}
	
	/*Save other tokens*/
	return 1;
}
	


/*Prints an error message when no operand is found.
This function is called when no known operand is found in a line.*/
void no_operand_found() {
	
	printf("No known operand found\n");

	error_flag = 1;
}


/*Prints an error message when no hyphen sign is found in a string definition line.
This function is called when no hyphen sign is found in a line that defines a string.*/
void no_hyphen_sign() {

	printf("on the string definition line there is no hyphen sign\n");

	error_flag = 1;
}


/*Prints an error message when a numeric constant is not found in the symbol table.
This function is called when a numeric constant is not found in the symbol table.*/
void not_found_permanent() {

	printf("No numeric constant found in the symbol table\n");

	error_flag = 1;
}



/*Handles error detection in operands based on the action and operands.
This function checks for errors in operands based on the action and operands provided.*/
void error_detection_in_operands(int action, int source_operand, int target_operand) {

	int flag = 0;

	/*Check for specific conditions based on the action and operands*/
	if(action >= 0 && action <= 5) {

		if(action == 0 || (action >= 2 && action <= 5) || (action >= 7 && action <= 8) || action == 11) {

			if(target_operand == 0) {
				
				flag = 1;
			}
		}

	} else if(action == 6) {

		if(target_operand == 0 || source_operand == 0 || source_operand == 3) {
			flag = 1;
		}

	} else if((action >= 9 && action <= 10) || action == 13) {
		
		if(target_operand == 0 || target_operand == 2) {

			flag = 1;
		}
	}

	if((action >= 4 && action <= 5) || (action >= 7 && action <= 15)) {
		
		if(source_operand != 0) {

			flag = 1;
		}
		
	}

	/*Print error message and set error flag if flag is set*/
	if(flag) {

		printf("Using an invalid operand for the desired operation\n");

		error_flag = 1;
	}
}
	


/*Processes a line to define a constant.
This function processes a line to define a constant and checks for validity.*/
int define_line(char *line, int line_number) {

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	token = strtok(NULL, " ");

	/*Check if token is invalid for constant name*/
	if(!save_words(token)) {

		printf("In line %d the name of the constant is invalid\n", line_number);
	
		free(save_line);

		/*Check if token is invalid for constant name*/
		return 1;
	}

	token = strtok(NULL, " ");

	/*Check if '=' sign is in the correct place*/
	if(*token != '=') {

		printf("In line %d the sign equals is not in the correct place for the pattern of a constant definition line\n", line_number);
	
		free(save_line);

		/*Return error code*/
		return 1;
	}

	token = strtok(NULL, " ");

	/*Check if token is not an integer*/
	if(is_not_integer(token)) {

		printf("In line %d, a constant definition line, there is an attempt to define a character that is not a constant number", line_number);

		free(save_line);

		/*Return error code*/
		return 1;
	}
		
	
	free(save_line);

	/*Return success code*/
	return 0;
}



/*Checks if a token is not an integer.
This function checks if a token contains characters that are not part of an integer.*/
int is_not_integer(char* token) {

	int i;

	for (i = 0; token[i] != '\0'; i++) {

		if(token[i] == '-' || token[i] == '+') {
		
			i++;
		}

        	if (!isdigit(token[i])) {

			/*Not an integer*/
           		return 1; 
        	}
    	}
	/*Is an integer*/
    	return 0; 
}



/*Determines the type of line based on the first token.
This function analyzes the first token of a line and determines the line type based on certain keywords.*/
int search_line_type(char *line) {

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);

	token = strtok(save_line, " ");

	if(strcmp(token, ".define") == 0) {

		free(save_line);

		return 0;

	} else if(strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {

		free(save_line);

		return 1;

	} else if(strcmp(token, ".data") == 0) {

		free(save_line);
		
		return 2;

	} else if(strcmp(token, ".string") == 0) {

		free(save_line);

		return 3;

	} else if(search_op_code(token) != -1) {

		free(save_line);

		return 4;
	}

	token = strtok(NULL, " ");

	if(strcmp(token, ".define") == 0) {

		free(save_line);

		return 0;

	} else if(strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {

		free(save_line);

		return 1;

	} else if(strcmp(token, ".data") == 0) {

		free(save_line);
		
		return 2;

	} else if(strcmp(token, ".string") == 0) {

		free(save_line);

		return 3;

	} else if(search_op_code(token) != -1) {

		free(save_line);

		return 4;
	}

	free(save_line);

	return -1;
}



/*Checks if a label, register, or opcode is already defined.
This function checks if a label, register, or opcode is already defined in the system.*/
int double_definition(char *curr_word) {

	if(find_label(curr_word) != -1) {
		printf("Attempting to define a label with an already defined name.\n");
		
		/*Return flag for double definition*/
		return 0;

	} else if(search_reg(curr_word) != -1 || search_op_code(curr_word) != -1){
		printf("An attempt to define a label in the name of a reserved word of the system\n");

		/*Return flag for reserved word definition*/
		return 0;
	}
	
	/*No double definition*/
	return 1;
}



/* Prints an error message for using a label at the beginning of an entry or extern line.
This function prints an error message when a label is used at the beginning of an entry or extern line.*/
void wrong_label() {

	printf("Using a label at the beginning of an entry or extern line is meaningless\n");
	
	error_flag = 1;

}




/*Prints an error message for lines that exceed the maximum line length.
This function prints an error message when a line exceeds the maximum allowed length.*/
void too_long_line(char *line, int line_number) {

	if(strlen(line) > MAX_LINE_LENGTH) {
	
		printf("line %d is too long! \n", line_number);

		error_flag = 1;

	}	
}



/*Prints an error message for defining a macro twice.
This function prints an error message when attempting to define a macro twice.*/
void define_macro_twice(char *macro_name) {

	int check;

	if((check = find_macro(macro_name)) != -1) {

		printf("macro %s has been defined twice", macro_name);

		error_flag = 1;

	}
}



/* Checks if a label name is also a macro name.
This function checks if a label name matches a macro name in the system.*/
void label_mcr_name(char *label_name) {

	if(find_macro(label_name) != -1) {

		printf("A label name cannot be the same name as an expanded macro\n");
		error_flag = 1;
	}
}
	



/*Checks if a word is null after memory allocation.
This function checks if a word is null after memory allocation, indicating a memory allocation failure*/
int null_word(char *word) {

	if(word == NULL) {

		printf("Memory allocation failed.\n");

		return 0;
	}

	return 1;
}
	




