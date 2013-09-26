#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "config.h"

#define TRUE 1
#define FALSE 0

int main(void) { 

	printf("\n");

	if (git_isInsideWorkTree())
		printf("Is in working tree\n");	

	printf("Ticketing system configurations: \n");

	struct git_ticket_config *config = git_parseConfig(TRUE);

	assert(strcmp(config->name, "ariel") == 0);
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

	return 0;
}
