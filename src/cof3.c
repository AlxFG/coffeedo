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
void str_parselist(struct Node *node,  char *string, size_t strsize);
void list_increment(struct Node **node);

int
cof3_parse(FILE *input, struct Node *cof3list)
{
	char *buffer = NULL;
	size_t strsize;

	list_addheader(input, cof3list);
	buffer = str_process(input);
	strsize = strlen(buffer);
	str_null(buffer);
	str_parselist(cof3list, buffer, strsize);
	free(buffer);
	return 0;
}

void
str_parselist(struct Node *node,  char *string, size_t strsize)
{
	for (int i = 0; i != (strsize - 1); i++) {
        if (i == 0) {
            list_add(node, &string);

        } else if (string[i] == '\0') {
			char *tmp = &string[i + 1];
			list_add(node, &tmp);
		}
	}
}

char *
str_process(FILE *input)
{
	char *buffer = calloc(1024, sizeof(char));
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
	node->data = calloc(strlen(*string) + 1, sizeof(char));
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

void
list_addheader(FILE *input, struct Node *node)
{
	char *header = calloc(50, sizeof(char));
	fgets(header, MAXHEADER, input);
	header[strcspn(header, "\n")] = '\0';
	list_init(node, &header);
	free(header);
}

void
list_increment(struct Node **node)
{
	*node = (*node)->next;
}

