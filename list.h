#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BITS 15
#define BINARY_ACTION_SIZE 5
#define BINARY_OPERAND_SIZE 3
#define BINARY_NUMBER 13
#define BINARY_WORD_LENGTH 15
#define MAX_WORDS 4095


struct Line {

	char *string;

	int line_number;
};


typedef struct Line Line;

struct Node {

	Line data;

	struct Node *next;

};

typedef struct Node Node;


	
/*Creates a new Line structure with the given string and line number.This function allocates memory for a new Line structure, copies the string into it, and assigns the line number.*/
Line* create_line(char *new_string, int line_number);

/*Adds a new string to the linked list. This function creates a new node with the given string and line number and adds it to the end of the linked list*/
void add_string(Node **head, char *new_string, int line_number);

/*Frees memory allocated for a Line structure. This function frees the memory allocated for a Line structure.*/
void free_line(Line *line);

/*Frees memory allocated for the entire linked list. This function frees the memory allocated for all nodes and Line structures in the linked list.*/
void free_list(Node *head);

/*Inserts a new string into the linked list. This function inserts a new string with the given line number into the linked list.*/
void insert(Node **head, char *new_string, int line_number);

/*Builds a new data command by inserting the given words into the linked list.
This function inserts the words into the linked list starting from the given*/
void build_new_data_command(char *first_word, char *second_word, char *third_word, char *fourth_word, char *fifth_word, int IC, int i);

/*Builds a code binary command by converting the given line to binary and inserting it into the linked list.*/
void build_code_binary_comm(char *line, int IC);

/*Builds a data binary command by converting the given line to binary and inserting it into the linked list.*/
void build_data_binary_comm(char *line, int IC);

/*Inserts a word into the linked list at the specified instruction counter (IC).
This function is used by other functions to add binary commands or data to the linked list.*/
void build_command(char *word, int IC);

extern Node *head;

#endif

