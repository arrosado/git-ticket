#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"

struct Node
{
	char *key;
	char *value;
	struct Node *next;
};

struct Node *load(FILE *file) {
	struct Node *list = NULL, **nextp = &list;
	char buffer[1024];

	while (fgets(buffer, sizeof buffer, file) != NULL) {
		struct Node *node;

		node = (Node*)malloc(sizeof(struct Node) + strlen(buffer) + 1);
		node->key = strtok(strcpy((char*)(node+1), buffer), "=\r\n");
		node->value = strtok(NULL, "\r\n");
		node->next = NULL;
		*nextp = node;
		nextp = &node->next;
	}

	return list;
}


int main(int argc, char** argv) { 
	printf("git-ticket working correctly.");
	system("git config -l > in.list");
	
	FILE *file;
	char mode = 'r';
	struct Node *head, *node;

	file = fopen("in.list", &mode);
	assert(file != NULL);
	head = load(file);
	fclose(file);

	for (node = head; node != NULL; node = node->next)
			printf("key=>\"%s\", val=>\"%s\"\n", node->key, node->value);
	
	return 0;
}
