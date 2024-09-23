main: main.o macro_pass.o labels.o instruction.o error.o directives.o assembler.o convert_to_binary.o list.o
	gcc -g -Wall -pedantic -ansi main.o macro_pass.o labels.o instruction.o error.o directives.o assembler.o convert_to_binary.o list.o -o main
main.o: main.c macro_pass.h labels.h instruction.h error.h directives.h assembler.h convert_to_binary.h list.h
	gcc -c -g -Wall -pedantic -ansi main.c -o main.o
assembler.o: assembler.c assembler.h macro_pass.h labels.h instruction.h error.h directives.h convert_to_binary.h list.h
	gcc -c -g -Wall -pedantic -ansi assembler.c -o assembler.o
convert_to_binary.o: convert_to_binary.c convert_to_binary.h directives.h instruction.h labels.h error.h macro_pass.h list.h
	gcc -c -g -Wall -pedantic -ansi convert_to_binary.c -o convert_to_binary.o
list.o: list.c list.h convert_to_binary.h directives.h instruction.h labels.h error.h macro_pass.h
	gcc -c -g -Wall -pedantic -ansi list.c -o list.o
macro_pass.o: macro_pass.c macro_pass.h labels.h instruction.h error.h directives.h
	gcc -c -g -Wall -pedantic -ansi macro_pass.c -o macro_pass.o
labels.o: labels.c labels.h instruction.h error.h list.h
	gcc -c -g -Wall -pedantic -ansi labels.c -o labels.o
instruction.o: instruction.c instruction.h labels.h macro_pass.h error.h directives.h list.h
	gcc -c -g -Wall -pedantic -ansi instruction.c -o instruction.o
error.o: error.c error.h macro_pass.h labels.h instruction.h directives.h list.h
	gcc -c -g -Wall -pedantic -ansi error.c -o error.o
directives.o: directives.c directives.h macro_pass.h labels.h list.h
	gcc -c -g -Wall -pedantic -ansi directives.c -o directives.o

