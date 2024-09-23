#include "convert_to_binary.h"
#include "directives.h"
#include "instruction.h"
#include "labels.h"
#include "error.h"
#include "list.h"


Node *head = NULL;

/*Creates a new Line structure with the given string and line number.
This function allocates memory for a new Line structure, copies the string into it, and assigns the line number.*/
Line* create_line(char *new_string, int line_number) {

	Line *line = (Line*)malloc(sizeof(Line));

	if(line != NULL) {

		 line->string = (char*)malloc((strlen(new_string) + 1) * sizeof(char));
		
		if (line->string != NULL) {

			strcpy(line->string, new_string);

			line->line_number = line_number;
		} else {
			
			free(line);

			line = NULL;
		}
	}

	return line;
}


/*Adds a new string to the linked list.
This function creates a new node with the given string and line number and adds it to the end of the linked list*/
void add_string(Node **head, char *new_string, int line_number) {

	Node *new_node;

	Node *current;
	
	Line *line = create_line(new_string, line_number);

	if(line != NULL) {

		new_node = (Node*)malloc(sizeof(Node));

		if(new_node != NULL) {

			/*Store the Line structure in the new node.*/
			new_node->data = *line;

			/*Check if the linked list is empty.*/
			if(*head == NULL) {
				
				new_node->next = NULL;

				/*Set the new node as the head of the linked list.*/
				*head = new_node;

			} else {

				current = *head;

				while(current->next != NULL) {
		
					current = current->next;

				}

				/*Add the new node to the end of the linked list.*/
				current->next = new_node;

				new_node->next = NULL;
			}

		} else {
			
			/*Free memory if allocation for the new node failed.*/
			free(line->string);

			free(line);
		}
	}
}



/*Frees memory allocated for a Line structure.
This function frees the memory allocated for a Line structure.*/
void free_line(Line *line) {
	
	if(line != NULL) {

		free(line->string);
	
		free(line);
	}
}



/*Frees memory allocated for the entire linked list.
This function frees the memory allocated for all nodes and Line structures in the linked list.*/
void free_list(Node *head) {
	
	Node *temp;
	
	while(head != NULL) {
		
		temp = head;

		head = head->next;

		free_line(&temp->data);

	}
}


 
/*Inserts a new string into the linked list.
This function inserts a new string with the given line number into the linked list.*/
void insert(Node **head, char *new_string, int line_number) { 

	add_string(head, new_string, line_number);
	
}
		


/*Builds a new data command by inserting the given words into the linked list.
This function inserts the words into the linked list starting from the given instruction counter (IC).
The number of lines in the command determines how many words are inserted.*/
void build_new_data_command(char *first_word, char *second_word, char *third_word, char *fourth_word, char *fifth_word, int IC, int i) {

	int counter = 0;

	int lines = instructions[i].lines_number_in_command;
	
	/*Inserts the first word into the linked list.*/
	insert(&head, first_word, IC + counter);

	if(lines >= 2 && lines <= 5) {
		
		counter++;

		/*Inserts the second word if applicable.*/
		insert(&head, second_word, IC + counter);
	}

	if(lines >= 3 && lines <= 5) {

		counter++;

		/*Inserts the third word if applicable.*/
		insert(&head, third_word, IC + counter);
	}

	if(lines >= 4 && lines <= 5) {

		counter++;

		/*Inserts the fourth word if applicable.*/
		insert(&head, fourth_word, IC + counter);
	}

	if(lines == 5) {

		counter++;

		/*Inserts the fifth word if applicable.*/
		insert(&head, fifth_word, IC + counter);
	}
	
}
		
	

/*Builds a code binary command by converting the given line to binary and inserting it into the linked list.
This function determines the appropriate conversion based on the instruction action and operands.*/
void build_code_binary_comm(char *line, int IC) {
	
	int i;

	char first_word[BINARY_WORD_LENGTH];
	
	for(i = 0; i < instructions_counter; i++) {

		if(instructions[i].line_number == IC) {
			
			break;
		}
	}

	/*Builds the first binary command word.*/
	build_first_binary_command(instructions[i].action, instructions[i].source_operand, instructions[i].target_operand, first_word);

	if((instructions[i].action >= 0 && instructions[i].action <= 3) || instructions[i].action == 6) {

		/*Converts and inserts the command with two operands.*/
		tow_operand_to_binary(line, i, first_word, IC);

	} else if((instructions[i].action >= 4 && instructions[i].action <= 5) || (instructions[i].action >= 7 && instructions[i].action <= 13)) {
		
		/* Converts and inserts the command with one operand.*/
		one_operand_to_binary(line, i, first_word, IC);

	} else {

		/*Inserts the command without operands.*/
		insert(&head, first_word, IC);
	
	}
}



/*Builds a data binary command by converting the given line to binary and inserting it into the linked list.
This function determines the appropriate conversion based on the directive type and content.*/
void build_data_binary_comm(char *line, int IC) {
	
	int i,j;

	char *token;

	int lines;

	char *number_str;

	int counter = 0;

	char *save_line = malloc(strlen(line) + 1);

	char new_string[BINARY_WORD_LENGTH];

	strcpy(save_line,line);

	token = strtok(save_line, " ");

	/*Parsing the line to determine the type of data command.*/

	if(check_if_label(token)) {
		
		for(i = 0; i < directive_counter; i++) {

			if(IC == directives[i].line_number) {
			
				break;
			}
		}

		lines = directives[i].add_lines;

		for(j = 0; j < directives[i].add_lines; j++) {

			strcpy(new_string, int_to_binary_for_data(directives[i].command[j]));
	
			build_command(new_string, IC + j);
		
		}

	} else {

		if(strcmp(token, ".data") == 0) {
			
			number_str = change_define_field(save_line);

			lines = count_comma(number_str);

			token = strtok(number_str, ",");

			while(token != NULL) {

				strcpy(new_string, int_to_binary_for_data(atoi(token)));
				
				build_command(new_string, IC + counter);

				counter++;

				token = strtok(NULL, ",");
			}


		} else if(strcmp(token, ".string") == 0) {
			
			number_str = find_string(line);
		
			lines = strlen(number_str) + 1;
			
			for(i = 0; i < lines; i++) {
				
				strcpy(new_string, int_to_binary_for_data(number_str[i]));
				
				build_command(new_string, IC + i);
			}
		}

		free(number_str);

	}

	free(save_line);	
}



/*Inserts a word into the linked list at the specified instruction counter (IC).
This function is used by other functions to add binary commands or data to the linked list.*/
void build_command(char *word, int IC) {

	insert(&head, word, IC);

}
		




