#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define BASE_FOUR_SIZE 8
#define NEW_LINE_SIZE 21

/*Parses a line of code during the assembly process.*/
void line_pers(char *line, int *IC, int *DC);

/*Checks if a given token (string) represents a directive to skip in assembly code processing.*/
int skip_line(char *token);

/*Counts external symbols and updates their place values.*/
void count_externs(char *line, int IC);

/*Fixes entry symbols in the symbol table.*/
void fix_entrys();

/*Reads a line from a file*/
char *read_line(FILE *file);

/*Fixes data-related label numbers and directive line numbers.*/
void fix_dc(int *IC);

/*Performs the first pass of assembly on a given file.*/
int first_pass(FILE *file);

/*Determines the type of line in the assembly code.*/
int line_type(char *line);

/*Finds the instruction counter (IC) value for a given line in the assembly code.*/
void find_ic(char *line, int type_line, int  *IC);

/*Executes the second pass of the assembly process.*/
void second_pass(FILE *file);

/*Counts the total number of instructions in the program.*/
int count_instructions();

/*Counts the total number of directives in the program.*/
int count_directives();

/*Generates the object file based on the assembled program.*/
void production_ob_file(FILE *file_ob);

/*Converts a binary command to base four encoding.*/
char* convert_binary_to_base_four(char *command);

/*Builds the external symbols file.*/
void build_ext_file(FILE *file_ext);

/*Builds the entry symbols file.*/
void build_ent_file(FILE *file_ent);

#endif
