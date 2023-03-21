#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "cof3.h"

#define RATIO 0.06

int getargs(int argc, char **argv, char **input);
int command_parse(struct Node *node, int volume);

int
main(int argc, char **argv)
{
	if (argc < 2) {
		printf("usage ./coffeedo volume or usage ./coffeedo -i file volume\n");
		return 1;
	}

	char *input = NULL;

	if (getargs(argc, argv, &input)) {
		return 1;
	}

	FILE *coffee_config = fopen(input, "r");

	if (coffee_config == NULL) {
		fprintf(stderr, "File could not be opened\n");
		return 1;
	}

	struct Node *commands = calloc(1, sizeof(struct Node));

	if (commands == NULL) {
		fprintf(stderr, "Failed to allocate memeory\n");
		return 1;
	}

	cof3_parse(coffee_config, commands);
    command_parse(commands, 500);
	list_destroy(commands);
	fclose(coffee_config);
	return 0;
}

int
getargs(int argc, char **argv, char **input)
{
	int opt;

	while ((opt = getopt(argc, argv, ":if:i")) != -1) {
		switch(opt) {
			case 'i':
				break;
			case ':':
				printf("arguments need value");
				return 1;
			case '?':
				printf("%c invalid argument\n",opt);
				return 1;
		}
	}

	*input = argv[optind];
	return 0;
}

int
command_parse(struct Node *node, int volume)
{
    int coffee_grams = volume * RATIO;

    struct Node **head = &node;
    while (*head != NULL) {
        if (strcmp((*head)->data, "*Wait") == 0) {
            list_increment(head);
            printf("Wait %i seconds\n\n", atoi((*head)->data));

        } else if (strcmp((*head)->data, "*Bloom") == 0) {
            list_increment(head);
            int tmp = atoi((*head)->data) * volume / 100;
            printf("Bloom for %ig\n\n", tmp);

        } else if (strcmp((*head)->data, "*Pour") == 0) {
            list_increment(head);
            int tmp = atoi((*head)->data) * volume / 100;
            printf("Pour %ig of water\n\n", tmp);

        } else if (strcmp((*head)->data, "*Swirl") == 0) {
            list_increment(head);
            printf("Swirl the brewer\n\n");

        } else if (strcmp((*head)->data, "*Draw") == 0) {
            list_increment(head);
            printf("Let the water draw down\n\n");

        } else if ((*head)->data[0] == '#') {
            printf("%s\n\n", (*head)->data);
            printf("Grams of coffee required %d\n\n", coffee_grams);
        }

        list_increment(head);
    }
    return 0;
}
