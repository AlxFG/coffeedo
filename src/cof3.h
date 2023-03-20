#include <stdio.h>

struct Node {
	struct Node *next;
	char *data;
};

int cof3_parse(FILE *a, struct Node *commands);
void list_destroy(struct Node *node);
void list_output(struct Node *node);
void list_increment(struct Node **node);
