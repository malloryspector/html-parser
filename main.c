#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_FILE_WEB_LENGTH 100
#define MAX_USER_INPUT 1000000
#define TAG_MAX_INPUT_SIZE 10
#define OPENING_TAG 2
#define CLOSING_TAG 3
#define SINGLETON_TAG 4

/**
 * TAG ID CONSTANTS
 */
#define H1 -1
#define BOLD -2
#define ITALIC -3
#define PARAGRAPH -4
#define LIST_ITEM -5
#define UNORDERED_LIST -6
#define LINK -7
#define META -8
#define IMAGE -9
#define BREAK -10
#define DIV -11
#define H2 -12
#define H3 -13
#define UNSUPPORTED_TAG -14 // catch all for any tag not yet supported

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
	int tag_name;
	struct tag *next;
};

/*
 * Returns TRUE if the stack has no elements
 * Note: Function adapted from a class example
 */
int is_empty(struct tag_stack *stack) {
    return stack == NULL || stack->head == NULL;
}

/**
 * Push the new tag name onto the tag stack
 * Note: Function adapted from a class example
 */
struct tag_stack *push(int tag_name, struct tag_stack *stack) {
	if (is_empty(stack)) {
		// if the tag stack is empty then this is the first element
		stack->head = malloc(sizeof(struct tag));
		stack->head->tag_name = tag_name;
		stack->head->next = NULL;
		return stack;
	}
	struct tag *new_element = malloc(sizeof(struct tag));
	new_element->tag_name = tag_name;
	new_element->next = stack->head;
	stack->head = new_element;
	return stack;
}

/**
 * Returns the tag value on top of the stack and removes that tag from the stack.
 * Note: Function adapted from a class example
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
 * Note: Function adapted from a class example
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

/**
 * Compares the tag on top of the stack with the encountered closing tag
 */
int compare_tags(struct tag *t, int tag_name) {
	if (t->tag_name == tag_name) {
		return TRUE;
	} else {
		printf("\n-------------------------\n");
		printf(" An error has been found \n");
		printf("-------------------------\n\n");
		printf("A tag is missing in the html file.\n\n");
		return FALSE;
	}
}

/**
 * Helper for printing the emulated html
 */
void print_emulated_opening_tag(int tag) {
	// Note: Looked at how to use switch statements for this project
	// https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm
	switch(tag) {
		case H1:
			printf("#");
			break;
		case H2:
			printf("##");
			break;
		case H3:
			printf("###");
			break;
		case BOLD:
			printf("*");
      		break;
      	case ITALIC:
      		printf("_");
      		break;
      	case UNORDERED_LIST:
      	case PARAGRAPH:
      	case BREAK:
      		printf("\n");
      		break;
      	case LIST_ITEM:
      		printf("- ");
      		break;
      	default:
      		printf("");
      		break;
	}
}

/**
 * Helper for printing the emulated html
 */
void print_emulated_closing_tag(int tag) {
	// Note: Looked at how to use switch statements for this project
	// https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm
	switch(tag) {
		case BOLD:
			printf("*");
      		break;
      	case ITALIC:
      		printf("_");
      		break;
      	case H1:
      	case H2:
      	case H3:
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

/**
 * Converts the tag name to the constant ID associated with the tag
 */
int get_tag_id(char tag_name[TAG_MAX_INPUT_SIZE]) {
	if (strcmp(tag_name, "h1") == 0) {
		return H1;
	} else if (strcmp(tag_name, "b") == 0) {
		return BOLD;
	} else if (strcmp(tag_name, "i") == 0) {
		return ITALIC;
	} else if (strcmp(tag_name, "p") == 0) {
		return PARAGRAPH;
	} else if (strcmp(tag_name, "li") == 0) {
		return LIST_ITEM;
	} else if (strcmp(tag_name, "ul") == 0){
		return UNORDERED_LIST;
	} else if (strcmp(tag_name, "link") == 0) {
		return LINK;
	} else if (strcmp(tag_name, "meta") == 0) {
		return META;
	} else if (strcmp(tag_name, "img") == 0) {
		return IMAGE;
	} else if (strcmp(tag_name, "br") == 0) {
		return BREAK;
	} else if (strcmp(tag_name, "div") == 0) {
		return DIV;
	} else if (strcmp(tag_name, "h2") == 0) {
		return H2;
	} else if (strcmp(tag_name, "h3") == 0) {
		return H3;
	} else {
		return UNSUPPORTED_TAG;
	}

}

/*
 * Determines if an opening tag is a singleton or a regular opening tag
 */
int get_tag_type(int tag_id) {
	int tag_type;
	// Note: Looked at how to use switch statements for this project
	// https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm
	switch(tag_id) {
		case LINK:
		case META:
		case IMAGE:
		case BREAK:
			tag_type = SINGLETON_TAG;
			break;
		default:
			tag_type = OPENING_TAG;
			break;
	}
	return tag_type;
}

/**
 * Converts the tag constant ID to the tag name
 */
char * get_tag_name(int tag_name) {
	if (tag_name == H1) {
		return "h1";
	} else if (tag_name == BOLD) {
		return "b";
	} else if (tag_name == ITALIC) {
		return "i";
	} else if (tag_name == PARAGRAPH) {
		return "p";
	} else if (tag_name == LIST_ITEM) {
		return "li";
	} else if (tag_name == UNORDERED_LIST) {
		return "ul";
	} else if (tag_name == LINK) {
		return "link";
	} else if (tag_name == META) {
		return "meta";
	} else if (tag_name == IMAGE) {
		return "img";
	} else if (tag_name == BREAK) {
		return "br";
	} else if (tag_name == DIV) {
		return "div";
	} else if (tag_name == H2) {
		return "h2";
	} else if (tag_name == H3) {
		return "h3";
	} else {
		return "";
	}
}

/**
 * Emulates the HTML as text and prints the output to the console
 */
void print_html_as_text(char user_input[MAX_USER_INPUT]) {
	printf("\n----------------------------\n");
	printf(" HTML has passed all tests! \n");
	printf("----------------------------\n\n");

	int skip = FALSE;

	// helper variables to keep track of each tag
	char tag[10];
	int t;

	for (int i = 0; i < strlen(user_input) - 1; i++) {
		int j = i + 1;

		if (user_input[i] == '<' && user_input[j] == '!') {
			// skip printing DOCTYPE and comment tags
			skip = TRUE;
			continue;
		} else if (user_input[i] == '<') {
			// clear the tag array so nothing is left over from previous
			for (int k = 0; k < 10; k++) {
				tag[k] = 0;
			}
			t = 0;

			// get the tag type to emulate
			while (user_input[j] != '>') {
				if (user_input[j] == '/') {
					j++;
				} else if (user_input[j] == ' ') {
					break;
				} else {
					tag[t] = user_input[j];
					t++;
					j++;
				}
			}
			int tag_id = get_tag_id(tag);

			if (user_input[i + 1] != '/') {
				print_emulated_opening_tag(tag_id);
			} else {
				print_emulated_closing_tag(tag_id);
			}
			skip = TRUE;
		} else if (skip && user_input[i] != '>') {
			continue;
		} else if (user_input[i] == '>') {
			skip = FALSE;
		} else if (user_input[i] == '\t' || user_input[i] == '\n' || user_input[i] == '\r' || user_input[i] == '\f') {
			continue;
		} else if (user_input[i] == ' ' && user_input[i - 1] == ' ') {
			continue;
		} else {
			printf("%c", user_input[i]);
		}
	}
	printf("\n");
}

void print_stack_remainder(struct tag_stack *stack) {
	printf("\n--------------------------------------\n");
	printf(" The following errors have been found \n");
	printf("--------------------------------------\n\n");

	while(!is_empty(stack)) {
		struct tag *head = pop(stack);
		char * tag_name = get_tag_name(head->tag_name);
		if (head->tag_name != UNSUPPORTED_TAG) {
			printf("Missing closing </%s> tag.\n", tag_name);
		} else {
			printf("Missing unsupported closing tag.\n");
		}
	}
	printf("\n");
}

int main() {

	// get file for user input
	// Note: Taking input from file was adapted from the in-class example
	char input_to_parse[MAX_FILE_WEB_LENGTH];
	printf("Enter html file to parse: ");
	fgets(input_to_parse, MAX_FILE_WEB_LENGTH, stdin);

	int len = strlen(input_to_parse);
	if (input_to_parse[len - 1] == '\n') {
		input_to_parse[len - 1] = '\0';
	}

	FILE *file_input;
	file_input = fopen(input_to_parse, "r");
	if (file_input == NULL) {
		printf("No file '%s' found!\n", input_to_parse);
	}

	char user_input[MAX_USER_INPUT];
	int index = 0;
	while (TRUE) {
		if (feof(file_input)) {
			user_input[index] = '\0';
			break;
		} else {
			user_input[index] = fgetc(file_input);
			index++;
		}
	}
	fclose(file_input);

	int total_input_length = index;

	// create the tag stack and set its head to NULL
	struct tag_stack *stack = malloc(sizeof(struct tag_stack));
	stack->head = NULL;

	// helper variables to keep track of each tag
	char tag[10];
	int t;

	// begin parsing: loop through all user input
	for (int i = 0; i < total_input_length - 1; i++) {
		// helper variable to loop through user input without
		// affecting the rest of the user input parsing
		int j = i + 1;

		if (user_input[i] == '<' && user_input[j] != '/' && user_input[j] != '!') {
			// clear the tag array so nothing is left over from previous
			for (int k = 0; k < 10; k++) {
				tag[k] = 0;
			}
			t = 0;

			// if the opening '<' is not directly followed by a '/'
			// then this is either an opening tag or a singleton tag
			while (user_input[j] != '>') {
				if (user_input[j] == ' ') {
					break;
				}
				tag[t] = user_input[j];
				t++;
				j++;
			}

			int tag_id = get_tag_id(tag);
			int tag_type = get_tag_type(tag_id);

			if (tag_type == OPENING_TAG) {
				stack = push(tag_id, stack);
			}
		} else if (user_input[i] == '<' && user_input[j] == '/') {
			// clear the tag array so nothing is left over from previous
			for (int k = 0; k < 10; k++) {
				tag[k] = 0;
			}
			t = 0;

			// if the opening '<' is directly followed by a '/'
			// then this is a closing tag
			j++; // add one to skip adding the '/' with the tag name
			while (user_input[j] != '>') {
				tag[t] = user_input[j];
				t++;
				j++;
			}

			int tag_id = get_tag_id(tag);

			struct tag *head = pop(stack);

			if (!compare_tags(head, tag_id)) {
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
