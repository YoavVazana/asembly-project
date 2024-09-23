#include "directives.h"
#include "macro_pass.h"
#include "labels.h"
#include "convert_to_binary.h"
#include "list.h"
#include "error.h"


int entrys_counter = 0;

int externs_counter = 0;

int directive_counter = 0;

char guidelines[][7] = {".data", ".string", ".entry", ".extern"};

directive_command *directives = NULL;

entry_command *entrys = NULL;

extern_command *externs = NULL;



/*Builds an entry command structure.*/

 /* This function constructs an entry command structure based on the given line, adding it to the entrys array.*/
void build_entry_command(char *line, int *DC) {

	char *entry_name = strtok(line," ");

	entry_name = strtok(NULL, " ");

	/*Allocate memory for the entry command*/
	entrys = realloc(entrys, sizeof(entry_command) * (entrys_counter + 1));

	entrys[entrys_counter].tag_name = malloc(sizeof(char) * (strlen(entry_name)+1));

	strcpy(entrys[entrys_counter].tag_name, entry_name);

	entrys[entrys_counter].place = *DC; 
	
	entrys_counter++;
}



/*Builds an extern command structure.*/

 /*This function constructs an extern command structure based on the given line, adding it to the externs array.*/
void build_extern_command(char *line, int *DC) {

	char *extern_name = strtok(line," ");

	extern_name = strtok(NULL, " ");

	/*Allocate memory for the extern command*/
	externs = realloc(externs, sizeof(extern_command) * (externs_counter + 1));

	externs[externs_counter].tag_name = malloc(sizeof(char) * (strlen(extern_name)+1));

	strcpy(externs[externs_counter].tag_name, extern_name);

	externs[externs_counter].counter = 0;

	externs_counter++;
}


/*This function counts the number of elements separated by commas in the given line.*/
int count_numbers(char *line) {

	int i = 0, count = 0;
	
	while(line[i] != '\0') {

		if(line[i] == ',') {

			count++;
		}

		i++;
	}

	count++;

	return count;	
}



/*This function searches for a string enclosed in double quotes within the given line and returns the extracted string.*/
char *find_string(char *line) {

	char *end_quote;

	char *string;

	size_t length;

	char *save_line;

	char *start_quote;

	save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	start_quote = strchr(save_line, '"');

	/*If the opening letters are not found, this is an error*/
	if(start_quote == NULL) {

		no_hyphen_sign();
	}

	end_quote = strchr(start_quote + 1, '"');

	/*If the closing fractions are not found, this is an error*/
	if(end_quote == NULL) {

		no_hyphen_sign();
	}

	length = end_quote - (start_quote + 1);

	string = malloc(length + 1);

	if(string == NULL) { 

		printf("In string definition line the string is empty\n");
	}

	strncpy(string, start_quote + 1, length);

	string[length] = '\0';

	free(save_line);

	return string;
	
}



/*This function checks if the given word is a label (ends with ':') or an extern directive.*/
int check_if_label(char *curr_word) {
	
	if((isalpha(curr_word[0]) && curr_word[strlen(curr_word) - 1] == ':') ||(strcmp(curr_word,".extern") == 0)) {
		
		return 1;
	}
	return 0;
}



/*This function extracts numbers (including negative numbers) and separators (comma) from the given line and returns them as a string.*/
char *find_numbers(char *line) {

	int i = 0, j = 0;

	char *numbers = malloc(strlen(line) + 1);
	
	while (line[i] != '\0') {

		if (isdigit(line[i]) || line[i] == '-' || line[i] == ',') {

			numbers[j++] = line[i];
		}

		i++;
	}
	numbers[j] = '\0';

	return numbers;
}



/*This function modifies a field in a define directive line according to the label number associated with the field if it represents a label. It replaces label names with their corresponding label numbers.*/
char *change_define_field(char *numbers) {

	char *token;

	int check;

	char *res = malloc(strlen(numbers) + 1);

	char *save_numbers = malloc(strlen(numbers) + 1);

	strcpy(save_numbers, numbers);

	token = strtok(save_numbers, " ");
	
	/*Check if the token is a label*/
	if(check_if_label(token)) {

		token = strtok(NULL, " ");
		
	}
	
	token = strtok(NULL, ",");
	
	/*Check if the tokens is a number and if not replace the defined variable name with the saved number if defined*/
	while(token != NULL) {

		if(atoi(token) == 0) {

			check = find_label(token);

			if(check != -1) {

				sprintf(res,"%s%d",res,labels[check].label_number);

			} else {

				not_found_permanent();
				
			}
				
		}else {
				/*If there is only one number*/
				if(count_comma(numbers) == 0) {

					strcpy(res, token);
					
					break;
				}

				sprintf(res, "%s%s,", res, token);
		}
		
		token = strtok(NULL, ",");
	}

	if (strlen(res) > 0 && res[strlen(res) - 1] == ',') {

		res[strlen(res) - 1] = '\0';
	}

	free(save_numbers);

	/*Returns a string of the numbers that were in the row*/
	return res; 
}



/*This function processes a data instruction line, extracts the numbers or strings from it, and builds a command structure to represent the data instruction, and from there the function updates the instruction table with the new command.*/
int build_data_command(char *line, int data_type, int *DC) {

	int numbers[MAX_LINE_LENGTH];

	int line_numbers = 0;

	char *token;

	char *numbers_str;

	char *string;

	int i;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line); 
	
	if(data_type == 0){
		
		/*Process .data directive*/
		numbers_str = change_define_field(save_line);

		/*Check if only one number is present*/
		if(count_comma(numbers_str) == 0) {

			numbers[line_numbers++] = atoi(numbers_str);

		} else {

			token = strtok(numbers_str, ",");

			while(token != NULL) {

				numbers[line_numbers++] = atoi(token);

				token = strtok(NULL, ",");
			}

			free(numbers_str);

		}
		
	} else if(data_type == 1) {
		
		/*Process .string directive*/
		string = find_string(save_line);
		
		line_numbers = strlen(string) + 1;
		
		for(i = 0; i < line_numbers; i++) {

			/*Convert each character of the string to its ASCII value*/
			numbers[i] = string[i];
		}
		
		free(string);
	}
	
	/*Allocate memory for the directive command*/
	directives = realloc(directives, sizeof(directive_command) * (directive_counter + 1));

	directives[directive_counter].line = malloc(sizeof(char) * (strlen(line)+1));
	
	for(i = 0; i < line_numbers; i++) {

		directives[directive_counter].command[i] = numbers[i];
	}

	directives[directive_counter].add_lines = line_numbers;

	directives[directive_counter].line_number = *DC;

	strcpy(directives[directive_counter].line, line);
	
	directive_counter++;
	
	return line_numbers;
}
	


/*This function takes a directive line, its corresponding index, and the current
data counter (`DC`), and processes the line according to the directive guideline index.*/
void classification_guidelines(char *line, int i, int *DC) {
	
	switch(i) {
		case 0:
			/*Process .data directive*/
			*DC += build_data_command(line, i, DC);

			break;
		case 1:
			/*Process .string directive*/
			*DC += build_data_command(line, i, DC);

			break;
		case 2:
			/*Process .entry directive*/
			build_entry_command(line, DC);

			break;
		case 3:
			/* Process .extern directive*/
			build_extern_command(line, DC);

			break;
	}	
}
			
	

/*This function searches for a given directive keyword (`curr_word`) in the guidelines array. If the keyword is found, it returns the index of the directive in the guidelines array.*/
int search_directive(char *curr_word) {

	int i;

	for(i = 0; i < GUIDELINES_ARR_LENGTH; i++) {

		if(strcmp(curr_word,".string") == 0 && i == 1) {
			
			return i;
		}

		if(strcmp(curr_word, guidelines[i]) == 0){
			
			return i;	
		}
	}

	return -1;
}



/*This function frees the memory allocated for directive structures such as entry commands, extern commands, and directive commands. It iterates through the arrays holding these structures and frees the memory for each element.*/
void free_directives() {

	int i;

	for (i = 0; i < entrys_counter; i++) {

		free(entrys[i].tag_name);
	}
	
	for (i = 0; i < externs_counter; i++) {

		free(externs[i].tag_name);
	}

	for(i = 0; i < directive_counter; i++) {
	
		free(directives[i].line);
	}

	free(directives);
}


