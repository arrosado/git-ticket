#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "config.h"

#define TRUE 1
#define FALSE 0

int main(void) 
{ 
	printf("Testing git configuration reading procedure.........");

	struct git_ticket_config *config = git_parseConfig(TRUE);

	assert(git_isInsideWorkTree() == TRUE);
	assert(strcmp(config->name, "Ariel Rosado Rivera") == 0);	
	assert(strcmp(config->repo, "git-ticket.git") == 0);
	assert(strcmp(config->service, "github") == 0);
	assert(config->ssl == FALSE);
	assert(strcmp(config->format_list, "") == 0);
	assert(strcmp(config->format_show, "") == 0);
	assert(strcmp(config->format_comment, "") == 0);
	assert(strcmp(config->gtoken, "") == 0);
	assert(strcmp(config->btoken, "") == 0);
	assert(strcmp(config->btoken_secret, "") == 0);
	assert(strcmp(config->rurl, "http://mcs.redmine") == 0);
	assert(strcmp(config->rpassword, "password") == 0);
	assert(strcmp(config->rtoken, "AiS8Ak34@ksd&js") == 0);
	assert(strcmp(git_guess_repo_name(), "git-ticket.git") == 0);
	assert(strcmp(git_guess_service(), "github") == 0);

	printf("OK\n");

	return 0;
}
