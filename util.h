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

struct Git
{
	struct Node *head, 
				*node,
				*list,
				**nextp;
};

void git_initialize(Git *git);

char *git_getConfig(Git *git, const char *key);

void git_dispose(Git *git);

struct Node *git_loadConfig(Git *git, FILE *file);

int string_indexof(char *str, char find);

#endif
