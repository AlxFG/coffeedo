/* Cof3 config parsing library */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXHEADER 50

struct Node {
	struct Node *next;
	char *data;
};

int list_init(struct Node *node, char **string);
void list_destroy(struct Node *node);
void list_output(struct Node *node);
void list_add(struct Node *node, char **string);
int list_addheader(FILE *input, struct Node *node);
void str_null(char *string);
char *str_process(FILE *input);
void str_parselist(struct Node *node,  char *string, size_t strsize);
void list_increment(struct Node **node);

/*
 * takes a file pointer and pointer to a struct to create a linked
 * list of strings
 */
int
cof3_parse(FILE *input, struct Node *cof3list)
{
	char *buffer = NULL;
	size_t strsize;

	if (list_addheader(input, cof3list)) {
		return 1;
	}
	buffer = str_process(input);
	strsize = strlen(buffer);
	str_null(buffer);
	str_parselist(cof3list, buffer, strsize);
	free(buffer);
	return 0;
}

/*
 * iterates through string to add them into linked list
 */
void
str_parselist(struct Node *node,  char *string, size_t strsize)
{
    list_add(node, &string);
	for (int i = 0; i != (strsize - 1); i++) {
        if (string[i] == '\0') {
			char *tmp = &string[i + 1];
			list_add(node, &tmp);
		}
	}
}

/*
 * reads from file pointer then adds them to a buffer
 */
char *
str_process(FILE *input)
{
	char *buffer = calloc(1024, sizeof(char));
	if (!buffer) {
		return NULL;
	}
	char curr, prev;
	curr = prev = '\0';
	int i;
	for (i = 0; (curr = fgetc(input)) != EOF; i++) {
		if (curr != '\n') {
			buffer[i] = curr;
		} else if (prev != '\n' && i != 0) {
			buffer[i] = ' ';
		} else {
			i--;
		}
		prev = curr;
	}
	buffer[i] = '\0';
    return buffer;
}

/*
 * replace spaces with null terminators to create multiple strings
 */
void
str_null(char *string)
{
	for (int i = 0; string[i] != '\0'; i++) {
		if (string[i] == ' ') {
			string[i] = '\0';
		}
	}
	return;
}

/*
 * adds first element to linked list
 */
int
list_init(struct Node *node, char **string)
{
	node->data = calloc(strlen(*string) + 1, sizeof(char));
	if (!(node->data)) {
		return 1;
	}
	node->next = NULL;
	strcpy(node->data, *string);
	return 0;
}

void
list_add(struct Node *node, char **string)
{
	if (node == NULL) {
		return;
	}

	list_add(node->next, string);

	if (node->next == NULL) {
		node->next = calloc(1, sizeof(struct Node));
		node->next->data = calloc(strlen(*string) + 1, sizeof(char));
		node->next->next = NULL;
		strcpy(node->next->data, *string);
	}
	return;
}

void
list_destroy(struct Node *node)
{
	if (node == NULL) {
		return;
	}
	list_destroy(node->next);
	free(node->data);
	free(node);
	return;
}

void
list_output(struct Node *node)
{
	if (node == NULL) {
		return;
	}
	printf("%s\n", node->data);
	list_output(node->next);
	return;
}

/*
 * adds header to linked list
 */
int
list_addheader(FILE *input, struct Node *node)
{
	char *header = calloc(50, sizeof(char));
	if (!header) {
		return 1;
	}
	fgets(header, MAXHEADER, input);
	header[strcspn(header, "\n")] = '\0';
	if (list_init(node, &header)) {
		return 1;
	}
	free(header);
	return 0;
}

/*
 * increments an indirect pointer to linked list
 */
void
list_increment(struct Node **node)
{
	*node = (*node)->next;
}

