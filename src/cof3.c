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

void list_init(char **string, struct Node *node);
void list_destroy(struct Node *node);
void list_output(struct Node *node);
void list_add(char **string, struct Node *node);
void str_null(char *string);

void
tokenise(FILE *input)
{
	struct Node *commands = calloc(1, sizeof(struct Node));
	char *header = calloc(50, sizeof(char));
	char *buffer = calloc(1024, sizeof(char));
	char curr, prev;
	int i;
	size_t strsize;

	fgets(header, MAXHEADER, input);
	header[strcspn(header, "\n")] = '\0';

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

	strsize = i;
	str_null(buffer);
	list_init(&header, commands);
	for (int j = 0; j != (strsize - 1); j++) {
		if (buffer[j] == '\0') {
			char *tmp = &buffer[j + 1];
			list_add(&tmp, commands);
		}
	}
	list_output(commands);
	list_destroy(commands);
	return;
}

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

void
list_init(char **string, struct Node *node)
{
	node->data = calloc(strlen(*string), sizeof(char));
	node->next = NULL;
	strcpy(node->data, *string);
}

void
list_add(char **string, struct Node *node)
{
	if (node == NULL) {
		return;
	}

	list_add(string, node->next);

	if (node->next == NULL) {
		node->next = calloc(1, sizeof(struct Node));
		node->next->data = calloc(strlen(*string), sizeof(char));
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
