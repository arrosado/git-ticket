#ifndef __CONFIG_H__
#define __CONFIG_H__

struct git_ticket_config 
{
	// Basic information.
	const char *name;
	const char *repo;
	const char *service;

	// SSL.
	int ssl;

	// List and show formatting.
	const char *format_list;
	const char *format_show;
	const char *format_comment;

	// Github specific.
	const char *gtoken;

	// Bitbucket specific.
	const char *btoken;
	const char *btoken_secret;

	// Redmine specific.
	const char *rurl;
	const char *rpassword;
	const char *rtoken;

};

int git_isInsideWorkTree(void);

int string_indexof(char *str, char find);

char **git_branches(void);

char *git_dir(void);

struct git_ticket_config *git_parseConfig(int doVerify);

char *git_guess_service(void);

char *git_guess_repo_name(void);

int isurl(char *url);

#endif
