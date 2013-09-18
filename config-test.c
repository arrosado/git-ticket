#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "config.h"

int main(void) { 

	git_initialize();

	printf("\n");

	if (git_getConfig("user.name"))
		printf("Username is %s\n", git_getConfig("user.name"));

	if (git_getConfig("user.email"))
		printf("User email is %s\n", git_getConfig("user.email"));

	if (git_isInsideWorkTree())
		printf("Is in working tree\n");	

	printf("Available branches are :\n");

	char **branches = git_branches();
	
	char *branch = NULL;
	int i = 0;
	while((branch = branches[i++]) != NULL)
		printf("%s\n", branch);

	printf("git dir is %s\n", git_dir());

	printf("Ticketing system configurations: \n");

	
	GitTicketConfig *config = git_parseConfig(true);

	printf("ticket.name: %s\n", config->name);
	printf("ticket.repo: %s\n", config->repo);
	printf("ticket.service: %s\n", config->service);
	printf("ticket.ssl: %d\n", config->ssl);
	printf("ticket.format.list: %s\n", config->format_list);
	printf("ticket.format.show: %s\n", config->format_show);
	printf("ticket.format.comment: %s\n", config->format_comment);
	printf("ticket.github.token: %s\n", config->gtoken);
	printf("ticket.bitbucket.token: %s\n", config->btoken);
	printf("ticket.bitbucket.token-secret: %s\n", config->btoken_secret);
	printf("ticket.redmine.url: %s\n", config->rurl);
	printf("ticket.redmine.password: %s\n", config->rpassword);
	printf("ticket.redmine.token: %s\n", config->rtoken);

	printf("repo name : %s\n", git_guess_repo_name());
	printf("service name : %s\n", git_guess_service());

	git_dispose();
	
	return 0;
}
