#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

struct Node *head, *node;

int string_indexof(char *str, char find) {
	return strchr(str, find) - str;
}

struct Node *git_loadConfig(FILE *file) {
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

void git_initialize()
{
	system("git config -l > in.list");

	FILE *file;
	char mode = 'r';

	file = fopen("in.list", &mode);
	assert(file != NULL);
	head = git_loadConfig(file);
	fclose(file);
}

void git_dispose(){
	if (head != NULL)
		free (head);
}

char *git_getConfig(const char *key)
{
	for (node = head; node != NULL; node = node->next)
		if (strcmp(node->key, key) ==  0)
			return node->value;

	return NULL;
}

bool git_isInsideWorkTree()
{
	system("git rev-parse --is-inside-work-tree > in.states");

	FILE *file;
	char mode = 'r';
	char buffer[255];

	file = fopen("in.states", &mode);

	assert(file != NULL);

	assert(fgets(buffer, sizeof buffer, file) != NULL);

	fclose(file);

	return strcmp(buffer, "true") == 0;
}
