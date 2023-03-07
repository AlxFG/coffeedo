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

void list_init(struct Node *node, char **string);
void list_destroy(struct Node *node);
void list_output(struct Node *node);
void list_add(struct Node *node, char **string);
void list_addheader(FILE *input, struct Node *node);
void str_null(char *string);
char *str_process(FILE *input);

int
tokenise(FILE *input)
{
	struct Node *commands = calloc(1, sizeof(struct Node));
	size_t strsize;

	list_addheader(input, commands);
    char *buffer = str_process(input);
	strsize = strlen(buffer);
	str_null(buffer);

	for (int j = 0; j != (strsize - 1); j++) {
		if (buffer[j] == '\0') {
			char *tmp = &buffer[j + 1];
			list_add(commands, &tmp);
		}
	}
	list_output(commands);
	list_destroy(commands);
	free(buffer);
	return 0;
}

char *
str_process(FILE *input)
{
	char *buffer = calloc(1024, sizeof(char));
	char curr, prev;
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
list_init(struct Node *node, char **string)
{
	node->data = calloc(strlen(*string), sizeof(char));
	node->next = NULL;
	strcpy(node->data, *string);
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

void
list_addheader(FILE *input, struct Node *node)
{
	char *header = calloc(50, sizeof(char));
	fgets(header, MAXHEADER, input);
	header[strcspn(header, "\n")] = '\0';
	list_init(node, &header);
	free(header);
}
