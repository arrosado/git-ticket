#ifndef __CONFIG_H__
#define __CONFIG_H__

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
	int ssl;

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

void git_initialize(void);

char *git_getConfig(const char *key);

void git_dispose(void);

struct Node *git_loadConfig(FILE *file);

int git_isInsideWorkTree(void);

int string_indexof(char *str, char find);

char **git_branches(void);

char *git_dir(void);

struct GitTicketConfig *git_parseConfig(int doVerify);

char *git_guess_service(void);

char *git_guess_repo_name(void);

int isurl(char *url);

#endif
