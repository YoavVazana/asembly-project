#include "macro_pass.h"


int macro_count = 0;

macro *macros = NULL;



/*Reads a line from a file and performs preprocessing for macros.
This function removes leading and trailing spaces, and adjusts spaces around commas.*/
char *read_line_for_macro(FILE *file) {
	char *line = malloc(sizeof(char)*MAX_LINE_LENGTH);
	if(fgets(line, MAX_LINE_LENGTH, file) == NULL) {
		return NULL;
	}

	/*Remove newline character.*/
	line[strcspn(line, "\n")] = '\0';

	keep_one_space(line);
		
	remove_space_before_comma(line);
		
	remove_space_after_comma(line);
	
	return line;
	
}



/*Extracts the macro name from a line containing a macro definition.*/
void mac_name(char *line, char *macro_name){
	char *token = strtok(line, " ");
	if(token != NULL && strcmp(token, "mcr") == 0) {
		token = strtok(NULL, " ");
		if(token != NULL) {
			strcpy(macro_name, token);
		}
	}	
}


/*Checks if a line contains a macro definition.*/
int is_macro(char *line) {
	return strncmp(line, "mcr", 3) == 0;
}



/*Finds a macro by its name in the macros array.*/
int find_macro(char *name) {
	int i;
	for(i = 0; i < macro_count; i++) {
		if(strcmp(macros[i].name, name) == 0) {
			return i;
		}
	}
	
	return -1;
}


/*Adds a macro definition to the macros array.*/
void add_macro(char *name, char *definition, int macro_size) {

	macros = realloc(macros, sizeof(macro) * (macro_count + 1));
	
	macros[macro_count].definition = malloc(sizeof(char)*(macro_size * MAX_LINE_LENGTH));
	
	strcpy(macros[macro_count].name, name);

	strcpy(macros[macro_count].definition, definition);

	macro_count++;

	
	
}
			


/*Replaces a macro name with its definition in a given line.
   - Searches for the macro name in the macros array.
   - If found, replaces the macro name with its definition.
   - Handles multiline macros.
   - Returns the modified line or the original line if the macro is not found.*/				
char *replace_macro(char *line) {

	char *macro_name = strtok(line, "\n");

	int macro_index = find_macro(macro_name);

	char *definition;

	char *new_line;

	/*If macro found.*/
	if(macro_index != -1) {

		definition = macros[macro_index].definition;

		new_line = malloc(strlen(definition) + strlen(line) - strlen(macro_name) + 1);

		strcpy(new_line, definition);

		strcat(new_line, " ");

		strcat(new_line, line + strlen(macro_name));

		/*Return the line with the macro replaced by its definition.*/
		return new_line;
		
	} else {
		/*Return the original line if the macro is not found.*/
		return line;
	}
}
	
	

/*Expands macros in a given input file and writes the expanded output to another file.
  - Handles multiline macros by reading lines until "endmcr" is encountered.
  - Replaces macro names with their definitions using replace_macro function.
  - Adds defined macros to the macros array.
  - Writes the expanded lines to the output file.*/
void expand_macros(FILE *file_in, FILE *file_out) {

	char *line;

	int in_macro = 0;

	char macro_name[MAX_MACRO_NAME];

	char macro_definition[MAX_MAC_DEF];

	char *new_line;

	int lines_in_macro = 0;

	int check;

	char *original_line;
	
	while ((line = read_line_for_macro(file_in)) != NULL) {
			
		/*Inside a multiline macro.*/
		if(in_macro) {
			
			if(strcmp(line, "endmcr") == 0) {
				
				/*Add macro to the macros array.*/
				add_macro(macro_name, macro_definition,lines_in_macro);

				in_macro = 0;
			} else {

				/*Append line to the macro definition.*/
				strcat(macro_definition, line);

        			strcat(macro_definition, "\n");
			}

		} else {

			/*Check if line is a macro definition.*/
			if(is_macro(line)) {

				/*Extract macro name.*/
				mac_name(line,macro_name);

				lines_in_macro++;

				in_macro = 1;

				strcpy(macro_definition, "");

			/*Check if line contains a defined macro.*/
			}else if((check = find_macro(line)) > -1){

				/*Write the macro definition to the output file.*/
				new_line = macros[check].definition;

				fprintf(file_out, "%s", new_line);
				
			} else {

				original_line = line;

				/*Replace macros in the line.*/
				new_line = replace_macro(line);

				/*Write the modified line to the output file.*/
				fprintf(file_out, "%s\n", new_line);

				if(new_line != original_line){

					free(new_line);
				}
			}
		}
		
		free(line);
		
		
	}

	/*Check for unclosed macros.*/
	if(in_macro) {

		printf("Error Macro did not close properly\n");

		return;
	}

}


/*This function removes spaces before commas in a given text.*/
void remove_space_before_comma(char *text) {

	char *src = text;

	char *dest = text;

	while (*src) {
		if (*src == ' ' && *(src + 1) == ',') {

			src++;
 
		} else {

			*dest++ = *src++; 
		}
	}

	*dest = '\0'; 
}


/*This function removes spaces after commas in a given text.*/
void remove_space_after_comma(char *text) {

	char *src = text;

	char *dest = text;

	int afterComma = 0; 

	while (*src) {

		if (*src == ',') {

			*dest++ = *src++; 

			afterComma = 1; 

		} else if (*src == ' ' && afterComma) {

			src++; 

		} else {

			*dest++ = *src++;
 
			afterComma = 0; 
		}
	}

	*dest = '\0'; 
}


/*This function keeps only one space between words in a given text.*/
void keep_one_space(char *text) {

	char *src = text;

	char *dest = text;

	int isSpace = 0; 

	while (*src == ' ' || *src == '\t') {

        	src++;
	}

	while (*src) {

		if (*src == ' ') {

			if (!isSpace) {

				*dest++ = ' ';
 
				isSpace = 1; 
			}

		} else {

			*dest++ = *src;
 
			isSpace = 0; 
		}

		src++;
	}

	*dest = '\0'; 
}



/*This function frees the memory allocated for macros and their definitions.*/
void free_macros() {

	int i;

	for(i = 0; i < macro_count; i++) {

		free(macros[i].definition);
	}

	free(macros);
}





