#include "macro_pass.h"
#include "labels.h"
#include "instruction.h"
#include "error.h"
#include "directives.h"
#include "assembler.h"
#include "convert_to_binary.h"
#include "list.h"


/*This file is a main program for assembly software in C language*/


/*This file contains the main logic for assembling a program using a two-pass algorithm.
It reads input files, expands macros, performs first and second passes, generates output files,
and handles error checking and memory deallocation.*/

int main(int argc, char *argv[])
{

	FILE *ptr_in;

	FILE *ptr_out;

	char output_file[FILE_NAME_LENGTH];

	char ob_file[FILE_NAME_LENGTH];

	char ext_file[FILE_NAME_LENGTH];

	char ent_file[FILE_NAME_LENGTH];

	int i;  /*Loop counter*/

	int flag = 0;  /*Flag for error checking*/


	/*Passes the arguments we received on the command line*/
	for(i = 1; i < argc; i++) {
	
		ptr_in = fopen(argv[i], "r");

		/*Check if file opening was successful*/
		if(ptr_in == NULL) {

			printf("Error!! The file cannot be read!\n");

			/*Continue to the next argument*/
			continue;
		}
		
		/*Generate output file name*/
		sprintf(output_file, "%s.am", argv[i]);

		ptr_out = fopen(output_file,"w");
		
		/*Expand macros in the input file*/
		expand_macros(ptr_in,ptr_out);

		fclose(ptr_in);

		fclose(ptr_out);

		ptr_out = fopen(output_file, "r");
		
		/*Perform first pass of assembly*/
		flag = first_pass(ptr_out);
		
		fclose(ptr_out);

		if(flag) {
			
			/*Free memory and continue to the next iteration*/
			free_macros();

			free_labels();

			free_directives();

			free_instructions();

			continue;
		}

		ptr_out = fopen(output_file, "r");
		
		/*Perform second pass of assembly*/
		second_pass(ptr_out);
		
		fclose(ptr_out);

		/*Generate object file*/
		sprintf(ob_file, "%s.ob", argv[i]);

		ptr_in = fopen(ob_file,"w");

		production_ob_file(ptr_in);

		fclose(ptr_in);


		/*Generate external symbols file if necessary*/
		if(externs_counter > 0) {

			sprintf(ext_file, "%s.ext", argv[i]);

			ptr_in = fopen(ext_file,"w");

			build_ext_file(ptr_in);

			fclose(ptr_in);
		}


		/*Generate entry symbols file if necessary*/
		if(entrys_counter > 0) {

			sprintf(ent_file, "%s.ent", argv[i]);

			ptr_in = fopen(ent_file,"w");

			build_ent_file(ptr_in);

			fclose(ptr_in);
		}

		
		/*Free memory for linked list and other resources*/
		free_list(head);

		free_macros();

		free_labels();

		free_directives();

		free_instructions();

	}

	return 0;
}
		


