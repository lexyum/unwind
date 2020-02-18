/* uwind --brainfuck to C interpreter.
 *
 * Alex Moore, October 2019
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// environment macros
const size_t DATA_SIZE = 30000;
const size_t READ_SIZE = 512;

void print_error()
{
	fprintf(stderr, "\033[1muwind: \033[1;31merror: \033[0m");
}

// loop stack
struct lnode {
	char *lstart;
	struct lnode *next;
};

struct lnode *push(struct lnode *head, char *i_ptr)
{
	struct lnode *tmp = malloc(sizeof(struct lnode));
	tmp->lstart = i_ptr;
	tmp->next = head;
	return tmp;
}

struct lnode *pop(struct lnode *head)
{
	struct lnode *tmp = head;
	head = head->next;
	free(tmp);

	return head;
}

void bfread(char *d_ptr)
{
		*d_ptr = getchar();
}

void bfprint(char *d_ptr)
{
		putchar(*d_ptr);
}

// Find matching '[' to ']' at i_ptr
char *lmatch(char *i_ptr)
{
	int depth = 1;

	while (depth != 0) {
		++i_ptr;
		if (*i_ptr == '[')
			++depth;
		else if (*i_ptr == ']')
			--depth;
	}
	return i_ptr;
}


void brainfuck(FILE **infile)
{

	char *i_start = calloc(READ_SIZE, sizeof(char));
	char *d_start = calloc(DATA_SIZE, sizeof(char));

	char *i_ptr = i_start;
	char *d_ptr = d_start;

	fread(i_ptr, sizeof(char), READ_SIZE, *infile);

	fclose(*infile);

	// head of loop stack
	struct lnode *head = NULL;
	
	while (*i_ptr != '\0') {
		switch (*i_ptr) {
		case '>' :
			++d_ptr;
			break;
		case '<' :
			--d_ptr;
			break;
		case '+' :
			++(*d_ptr);
			break;
		case '-' :
			--(*d_ptr);
			break;
		case '.' :
			bfprint(d_ptr);
			break;
		case ',' :
			bfread(d_ptr);
			break;
		   
		case '[' :
			// push i_ptr for loop start to stack
			if (*d_ptr == 0) {
				i_ptr = lmatch(i_ptr);
			}
			else {
				head = push(head, i_ptr);
			}
			break;
		case ']' :
			// pop stack once loop is broken
			if (*d_ptr != 0) {
				i_ptr = head->lstart;
			}
			else {
				head = pop(head);
			}
		}
		++i_ptr;
	}

	free(i_start);
	free(d_start);
}

// main() deals only with i/o.
// once we have an input stream, control passes to brainfuck()
int main(int argc, char **argv)
{
	if (argc == 1) {
		print_error();
		printf("no input file.\n");
		exit(EXIT_FAILURE);
	}
	else if (argc > 2) {
		print_error();
		printf("one argument [file] expected.\n");
		exit(EXIT_FAILURE);
	}

	const char *infile = argv[1];
	FILE *fptr;

		if ((fptr = fopen(infile, "r")) == NULL) {
		print_error();
		printf("%s: no such file or directory.\n", infile);
		exit(EXIT_FAILURE);
	}

	brainfuck(&fptr);

	exit(EXIT_SUCCESS);
}
