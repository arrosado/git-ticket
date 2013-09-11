#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int string_indexof(char *str, char find) {
	return strchr(str, find) - str;
}

void git_initialize(Git *git)
{
	git = (Git *)malloc(sizeof(struct Git) + sizeof(struct Node) * 4);
	system("git config -l > in.list");

	FILE *file;
	char mode = 'r';

	file = fopen("in.list", &mode);
	assert(file != NULL);
	git_loadConfig(git, file);
	fclose(file);
}

void git_dispose(Git *git){
	if (git != NULL)
		free (git);
}

char *git_getConfig(Git *git, const char *key)
{
	for (git->node = git->head; git->node != NULL; git->node = git->node->next)
		if (strcmp(git->node->key, key) ==  0)
			return git->node->value;

	return NULL;
}

struct Node *git_loadConfig(Git *git, FILE *file) {
	git->list = NULL;
	git->nextp = &git->list;
	char buffer[1024];

	while (fgets(buffer, sizeof buffer, file) != NULL) {
		git->node = (Node*)malloc(sizeof(struct Node) + strlen(buffer) + 1);
		git->node->key = strtok(strcpy((char*)(git->node+1), buffer), "=\r\n");
		git->node->value = strtok(NULL, "\r\n");
		git->node->next = NULL;
		*(git->nextp) = git->node;
		git->nextp = &(git->node->next);
	}

	return git->list;
}
