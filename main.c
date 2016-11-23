#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_USER_INPUT 1000000
#define TAG_MAX_INPUT_SIZE 10
#define OPENING_TAG 2
#define CLOSING_TAG 3
#define SINGLETON_TAG 4

/**
 * TAG CONSTANTS
 */
#define H1 -1
#define BOLD -2
#define ITALIC -3
#define PARAGRAPH -4
#define LIST_ITEM -5
#define UNORDERED_LIST -6
#define UNSUPPORTED_TAG -7 // catch all for any tag not yet supported

/** 
 * Struct to keep track of what is at the top of the stack
 */
struct tag_stack {
	struct tag *head;
};

/** 
 * Struct to keep track of the tag name and stack order
 */
struct tag {
	char tag_name[10];
	int tag_type;
	struct tag *next;
};

/* 
 * Returns TRUE if the stack has no elements 
 */
int is_empty(struct tag_stack *stack) {
    return stack == NULL || stack->head == NULL;
}

/**
 * Push the new tag name onto the tag stack
 */
struct tag_stack *push(char tag_name[TAG_MAX_INPUT_SIZE], int tag_type, struct tag_stack *stack) {
	if (is_empty(stack)) {
		// if the tag stack is empty then this is the first element
		stack->head = malloc(sizeof(struct tag));
		strcpy(stack->head->tag_name, tag_name);
		stack->head->tag_type = tag_type;
		stack->head->next = NULL;
		return stack;
	}
	struct tag *new_element = malloc(sizeof(struct tag));
	strcpy(new_element->tag_name, tag_name);
	new_element->tag_type = tag_type;
	new_element->next = stack->head;
	stack->head = new_element;
	return stack;
}

/**
 * Returns the tag value on top of the stack and removes that tag from the stack.
 */
struct tag *pop(struct tag_stack *stack) {
	struct tag *to_return = stack->head;
	struct tag *to_delete = stack->head;
	stack->head = stack->head->next;
	free(to_delete);
	return to_return;
}

/**
 * Free the entire stack before exiting the program.
 */
void free_stack(struct tag_stack *stack) {
	struct tag *t = stack->head;
	while (t != NULL) {
		struct tag *next = t->next;
		free(t);
		t = next;
	}
	free(stack);
}

int compare_tags(struct tag *t, char tag_name[TAG_MAX_INPUT_SIZE]) {
	if (strcmp(t->tag_name, tag_name) == 0) {
		return TRUE;
	} else {
		printf("--------------------------------------\n");
		printf(" The following errors have been found \n");
		printf("--------------------------------------\n\n");
		printf("Missing %s tag.\n\n", tag_name);
		return FALSE;
	}
}

void print_emulated_opening_tag(int tag) {
	switch(tag) {
		case H1:
			printf("#");
			break;
		case BOLD:
			printf("*");
      		break;
      	case ITALIC:
      		printf("_");
      		break;
      	case UNORDERED_LIST:	
      	case PARAGRAPH:
      		printf("\n");
      		break;
      	case LIST_ITEM:
      		printf("-");
      		break;	
      	default:
      		printf("");
      		break;
	}
}

void print_emulated_closing_tag(int tag) {
	switch(tag) {
		case BOLD:
			printf("*");
      		break;
      	case ITALIC:
      		printf("_");
      		break;
      	case H1:
      	case PARAGRAPH:
      	case UNORDERED_LIST:
      	case LIST_ITEM:
      		printf("\n");
      		break;	
      	default:
      		printf("");
      		break;
	}	
}

int get_tag_type(char tag_name[TAG_MAX_INPUT_SIZE]) {
	char * h1 = "h1";
	char * bold = "b";
	char * italic = "i";
	char * paragraph = "p";
	char * list_item = "li";
	char * unordered_list = "ul";

	if (strcmp(tag_name, h1) == 0) {
		return H1;
	} else if (strcmp(tag_name, bold) == 0) {
		return BOLD;
	} else if (strcmp(tag_name, italic) == 0) {
		return ITALIC;
	} else if (strcmp(tag_name, paragraph) == 0) {
		return PARAGRAPH;
	} else if (strcmp(tag_name, list_item) == 0) {
		return LIST_ITEM;
	} else if (strcmp(tag_name, unordered_list) == 0){
		return UNORDERED_LIST;
	} else {
		return UNSUPPORTED_TAG;
	}
}

void print_html_as_text(char user_input[MAX_USER_INPUT]) {
	printf("----------------------------\n");
	printf(" HTML has passed all tests! \n");
	printf("----------------------------\n\n");

	int skip = FALSE;

	for (int i = 0; i < strlen(user_input); i++) {
		char tag[10] = {0};
		int t = 0;
		int j = i + 1;

		if (user_input[i] == '<') {
			// get the tag type to emulate
			while (user_input[j] != '>') {
				if (user_input[j] == '/') {
					j++;
				} else {
					tag[t] = user_input[j];
					t++;
					j++;
				}
			}
			int tag_name = get_tag_type(tag);

			if (user_input[i + 1] != '/') {
				print_emulated_opening_tag(tag_name);
			} else {
				print_emulated_closing_tag(tag_name);
			}
			skip = TRUE;
		} else if (skip && user_input[i] != '>') {
			continue;
		} else if (user_input[i] == '>') {
			skip = FALSE;
		} else {	
			printf("%c", user_input[i]);
		}
	}
	printf("\n");
}

void print_stack_remainder(struct tag_stack *stack) {
	printf("--------------------------------------\n");
	printf(" The following errors have been found \n");
	printf("--------------------------------------\n\n");

	while(!is_empty(stack)) {
		struct tag *head = pop(stack);
		printf("Missing closing </%s> tag.\n", head->tag_name);
	}
	printf("\n");
}

int main() {

	// get HTML user input
	char user_input[MAX_USER_INPUT];
	printf("Enter an HTML string: ");
	fgets(user_input, MAX_USER_INPUT, stdin);

	// create the tag stack and set its head to NULL
	struct tag_stack *stack = malloc(sizeof(struct tag_stack));
	stack->head = NULL;

	// begin parsing: loop through all user input
	for (int i = 0; i < strlen(user_input); i++) {
		// helper variables to keep track of each tag
		char tag[10] = {0};
		int t = 0;

		// helper variable to loop through user input without 
		// affecting the rest of the user input parsing
		int j = i + 1;

		if (user_input[i] == '<' && user_input[j] != '/') {
			// if the opening '<' is not directly followed by a '/'
			// then this is either an opening tag or a singleton tag
			int tag_type = OPENING_TAG;
			while (user_input[j] != '>') {
				if (user_input[j] == '/') {
					tag_type = SINGLETON_TAG;
				}
				tag[t] = user_input[j];
				t++;
				j++;
			}
			if (tag_type == OPENING_TAG) {
				stack = push(tag, tag_type, stack);
			}
		} else if (user_input[i] == '<' && user_input[j] == '/') {
			// if the opening '<' is directly followed by a '/'
			// then this is a closing tag
			j++; // add one to skip adding the '/' with the tag name
			while (user_input[j] != '>') {
				tag[t] = user_input[j];
				t++;
				j++;
			}
			struct tag *head = pop(stack);
			if (!compare_tags(head, tag)) {
				return 0;
			}
		}
	}

	if (!is_empty(stack)) {
		// if we have looped through all user input and the stack is not empty
		// then there are missing closing tags in the HTML
		print_stack_remainder(stack);
	} else {
		// if there is nothing left in the stack, the HTML has been successfully
		// parsed and it is safe to try to emulate the html as text
		print_html_as_text(user_input);
	}

	free_stack(stack);
	return 0;
}



















