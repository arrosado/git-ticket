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

struct GitTicketConfig 
{
	// Basic information.
	char *name;
	char *repo;
	char *service;

	// SSL.
	bool ssl;

	// List and show formatting.
	char *format_list;
	char *format_show;
	char *format_comment;

	// Github specific.
	char *gtoken;

	// Bitbucket specific.
	char *btoken;
	char *btoken_secret;

	// Redmine specific.
	char *rurl;
	char *rpassword;
	char *rtoken;

};

void git_initialize();

char *git_getConfig(const char *key);

void git_dispose();

struct Node *git_loadConfig(FILE *file);

bool git_isInsideWorkTree();

int string_indexof(char *str, char find);

char **git_branches();

char *git_dir();

GitTicketConfig *git_parseConfig(bool doVerify);

char *git_guess_service();

char *git_guess_repo_name();

bool isurl(char *url);

#endif
