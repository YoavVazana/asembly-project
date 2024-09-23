#include "list.h"
#include "labels.h"
#include "instruction.h"
#include "error.h"

/* Array of feature types: mdefine, data, code */
char feature_type[][7] = {"mdefine", "data", "code"};

/* Array of ARE types: absolute, external, relocatable */
char are_type[][11] = {"absolute", "external", "relocatable"};

/* Counter for labels */
int label_counter = 0;

/* Array of labels */
label *labels = NULL;


/* Checks if a string represents a number directive (.define) */
int is_number(char *curr_word) {
	
	if(strcmp(curr_word,".define") == 0) {

		return 1;
	}

	return 0;
}


/* Checks if a line contains a label and determines its type */
int is_label(char *line) {

	char *save_line;

	char *curr_word;

	save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	/*Tokenize the line to check the first word*/
	curr_word = strtok(save_line, " ");
	
	/*Check if the first word is alphabetic and ends with a colon*/
	if(isalpha(curr_word[0]) && curr_word[strlen(curr_word) - 1] == ':') {
		
		curr_word = strtok(NULL, " ");
		
		/*Checks the data type*/
		if(strcmp(curr_word, ".entry") != 0 && strcmp(curr_word, ".extern") != 0){
		
			/*returns 1 if the label is of a line of type .string or .data*/
			if(strcmp(curr_word, ".string") == 0 || strcmp(curr_word, ".data") == 0) {
				
				free(save_line);

				return 1;

			/*returns 2 if the label is of a line of instruction type*/
			}else if(search_op_code(curr_word) > -1) {
				

				free(save_line);

				return 2;
			}

		
		} else if(strcmp(curr_word, ".entry") == 0 || strcmp(curr_word, ".extern") == 0) {
			wrong_label();

			/*returns 3 if the label is of a line of type .extern*/
			if(strcmp(curr_word, ".extern") == 0) {
				
				free(save_line);
	
				return 3;
			}

			/*returns -1 if we did not find*/
			free(save_line);

			return -1;
		}

	/*returns 3 if the label is of a line of type .extern*/
	}else if(strcmp(curr_word, ".extern") == 0) {
		
		free(save_line);

		return 3;
	}
		
	/*returns 0 if we did not find*/
	free(save_line);

	return 0;
}


/* Parses a line to extract a label's name and value from a .define directive */
void label_number_define(char *line, char *name, int *value) {

	char *token;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	/*Tokenize the line to extract label information*/
	token = strtok(save_line, " ");
	
	token = strtok(NULL, " ");

	/*Copy the label's name to the provided pointer*/
	strcpy(name, token);
	
	/*Tokenize further to extract the label's value*/
	token = strtok(NULL," ");

	token = strtok(NULL," ");

	*value = atoi(token);

	free(save_line);		
	
}
		
/* Extracts a label's name from a line */
void label_name_define(char *line, char *name) {

	char *token;
	
	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);
	
	/*Tokenize the line to extract the label's name*/
	token = strtok(save_line, ":");

	/*Copy the label's name to the provided pointer*/
	strcpy(name,token);

	free(save_line);
		
}
	

/* Builds the label table with the specified label name, number, feature, and ARE type */
void build_table_label(char *label_name, int *label_number, int feature, int are) {
	
	/*Reallocate memory for labels array to accommodate the new label*/	
	labels = realloc(labels, sizeof(label) * (label_counter + 1));

	/*Allocate memory for the label name and copy the name*/
	labels[label_counter].tag_name = malloc(sizeof(char) *(strlen(label_name)+1));
	
	strcpy(labels[label_counter].tag_name,label_name);

	/*Set the feature type and ARE type based on the provided parameters*/
	if(feature == 0) {

		strcpy(labels[label_counter].feature, "mdefine");

		labels[label_counter].are = 0;

		labels[label_counter].label_number = *label_number;

	} else if(feature == 3){

		labels[label_counter].feature[0] = '\0';

		labels[label_counter].are = 1;

		labels[label_counter].label_number = 0;
	
	} else {
		strcpy(labels[label_counter].feature, feature_type[feature]);

		labels[label_counter].are = are;

		labels[label_counter].label_number = *label_number;
	}
	
	label_counter++;
	
}



/* Finds the index of a label in the label table based on its name */
int find_label(char *curr_word) {

	int i;

	/*Iterate through the label table to find the matching label name*/
	for(i = 0; i < label_counter; i++) {

		if(strcmp(labels[i].tag_name, curr_word) == 0) {

			/*Return the index of the found label*/
			return i;

		}
	}

	/*Return -1 if the label is not found*/
	return -1;
}



/* Processes an .extern directive line, adding the external label to the label table */
void extern_line(char *line) {
	char *token;
	
	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line, line);

	token = strtok(save_line, " ");

	/*Check if the line is an .extern directive*/
	if(strcmp(token, ".extern") != 0) {
		
		token = strtok(NULL, " ");
	}

	token = strtok(NULL, " ");

	build_table_label(token, 0, 3, 1);
	
	free(save_line);
	
}

	

/* Searches for labels and processes label definitions in the input line */
void label_search(char *line, char *curr_word, int *IC, int *DC) {

	char label_word[MAX_LABEL_LENGTH];

	int label_place;

	int flag = 0;

	int feature;

	int are;

	char *save_line = malloc(strlen(line) + 1);

	strcpy(save_line,line);

	/*Check for double label definition*/
	if(!double_definition(curr_word)) {

		free(save_line);
		
		error_flag = 1;

		return;
	}
	
	/*Check if the current word is a number definition*/
	if(is_number(curr_word)) {

		feature = 0;

		are = 0;

		label_number_define(save_line, label_word, &label_place);

		flag = 1;

	/*Checks if the label is valid and the row type*/
	}else if((feature = is_label(save_line)) > 0) {

		if(feature == 3) {

			/*Extern type definition in the label table*/
			extern_line(line);
			
			free(save_line);
			
			return;
		}

		label_name_define(save_line, label_word);

		/*Determine feature and ARE type based on line type*/
		if(feature == 1) {

			are = 2;

			label_place = *DC;

		}else if(feature == 2) {

			are = 0;

			label_place = *IC;
		}

		flag = 1;

	} else {

		/*Not a label or number definition, free memory and return*/
		free(save_line);

		return;
	}
	
	/*If a valid label is found, add it to the label table*/	
	if(flag) {

		build_table_label(label_word, &label_place, feature, are);

		free(save_line);

		return;
	}
		
	free(save_line);

	return;
	
}



/* Frees allocated memory for label names and the label table */
void free_labels() {

	int i;

	for(i = 0; i < label_counter; i++) {
		
		if(strlen(labels[i].tag_name) > 1) {
			free(labels[i].tag_name);
		}
	}
	
	free(labels);
}







