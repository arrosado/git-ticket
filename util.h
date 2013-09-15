#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Node
{
	char *key;
	char *value;
	struct Node *next;
};

void git_initialize();

char *git_getConfig(const char *key);

void git_dispose();

struct Node *git_loadConfig(FILE *file);

bool git_isInsideWorkTree();

int string_indexof(char *str, char find);

char **git_branches();

#endif
