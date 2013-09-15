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

	return (strcmp(buffer, "true\n") == 0);
}

char **git_branches()
{
	system("git branch > in.branches && cat in.branches | wc -l > in.branchcount");

	FILE *file;
	char mode = 'r';
	char **branches;
	char buffer[255];
	int branchCount = 0;
	int i = 0;

	// Read the number of branches on the repo.
	// Set the number of branches into branch count.
	file = fopen("in.branchcount", &mode);

	assert(fgets(buffer, sizeof buffer, file) != NULL);

	branchCount = atoi(buffer);

	fclose(file);


	// Read the branch names using the branch count.
	file = fopen("in.branches", &mode);

	assert(file != NULL);

	branches = (char**)malloc(sizeof(char) * (branchCount+1));

	for (i = 0; i < branchCount; i++)
	{
		assert(fgets(buffer, sizeof buffer, file) != NULL);
		branches[i] = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		// Start copying from index 2 on, 
		// this will avoid coping the * mark 
		// that git adds to identify the current 
		// branch.
		// Also avoid copying the \n character 
		// that exists at the end of each line.
		//
		strncpy(branches[i], buffer+2, strlen(buffer) - 3);
	}

	fclose(file);

	branches[branchCount] = NULL;
	
	return branches;
}

char *git_dir() {
	system("git rev-parse -q --git-dir > in.gitdir");

	FILE *file;
	char mode = 'r';
	char buffer[255];
	char *dir;

	file = fopen("in.gitdir", &mode);

	assert(file != NULL);

	assert(fgets(buffer, sizeof buffer, file) != NULL);

	dir = (char*)malloc(sizeof(char) * strlen(buffer) + 1);

	strcpy(dir, buffer);

	fclose(file);

	return dir;
}

