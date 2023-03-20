#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "cof3.h"

int getargs(int argc, char **argv, char **input);

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
	list_output(commands);
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
