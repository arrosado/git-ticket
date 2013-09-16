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

	strncpy(dir, buffer, strlen(buffer) - 1);

	fclose(file);

	return dir;
}

GitTicketConfig *git_parseConfig(bool doVerify) {
	GitTicketConfig *config = (GitTicketConfig*)malloc(sizeof(struct GitTicketConfig));

	config->name = (git_getConfig("ticket.name")) ? git_getConfig("ticket.name") : git_getConfig("user.name");
	config->repo = (git_getConfig("ticket.repo")) ? git_getConfig("ticket.repo") : git_guess_repo_name();
	config->service = (git_getConfig("ticket.service")) ? git_getConfig("ticket.service") : git_guess_service();

	char *httpssl = git_getConfig("ticket.ssl");
	
	if (httpssl)
		config->ssl = strcmp(httpssl, "true") == 0;
	
	config->format_list = git_getConfig("ticket.format.list");
	config->format_show = git_getConfig("ticket.format.show");
	config->format_comment = git_getConfig("ticket.format.comment");
	config->gtoken = git_getConfig("ticket.github.token");
	config->btoken = git_getConfig("ticket.bitbucket.token");
	config->btoken_secret = git_getConfig("ticket.bitbucket.token-secret");
	config->rurl = git_getConfig("ticket.redmine.url");
	config->rpassword = git_getConfig("ticket.redmine.password");
	config->rtoken = git_getConfig("ticket.redmine.token");

	if (doVerify)
	{
		if (config->service == NULL) {
			printf("Can't guess a service. Try 'git config ticket.service [github|bitbucket|redmine]'\n");
			exit(1);
		}

		if (((strcmp(config->service, "github") == 0) ||
			 (strcmp(config->service, "bitbucket") == 0) ||
			 (strcmp(config->service, "redmine") == 0)) == 0)
		{
			printf("%s is a unknown service. You must choose a service from github, bitbucket, and redmine.\n", config->service);
			exit(1);
		}

		if (config->repo == NULL) {
			printf("Can't guess a repository name. Try 'git config ticket.repo <repository_name>'\n");
			exit(1);
		}

		if (config->name == NULL) {
			printf("You must set your account name to ticket.name or user.name. Try 'git config ticket.name <your_account_name>'\n");
			exit(1);
		}

		if (config->service && 
			strcmp(config->service, "redmine") == 0 &&
			config->rurl == NULL) {
			printf("You must set a URL of your redmine. Try 'git config ticket.redmine.url <redmine_url>'\n");
			exit(1);
		}
	}

	return config;
}

bool isurl(char *url) {
	return (strstr(url, "://") != NULL);
}

char *git_guess_repo_name()
{
	char *origin_url = git_getConfig("remote.origin.url");

	char *url = (char*)malloc(sizeof(char) * (strlen(origin_url) + 1));

	strcpy(url, origin_url);

	if (!url) 
		return NULL;

	char *repo_name = NULL;
	char *temp_name = strtok(url, "/");

	while ((temp_name = strtok(NULL, "/")) != NULL) 
		repo_name = temp_name;

	if (repo_name != NULL){
		free(url);
		return repo_name;
	}

	temp_name = strtok(url, ":");

	while ((temp_name = strtok(NULL, ":")) != NULL) 
		repo_name = temp_name;

	free(url);

	return repo_name;
}

char *git_guess_service() {
	char *origin_url = git_getConfig("remote.origin.url");

	char *url = (char*)malloc(sizeof(char) * (strlen(origin_url) + 1));

	strcpy(url, origin_url);

	if (!url) 
		return NULL;

	char *github = (char*)malloc(sizeof(char) * 7);
	strcpy(github, "github");

	char *bitbucket = (char*)malloc(sizeof(char) * 10);
	strcpy(bitbucket, "bitbucket");

	char *redmine = (char*)malloc(sizeof(char) * 8);
	strcpy(redmine, "redmine");

	char *value = NULL;

	value = strtok(url, "/");
	while ((value = strtok(NULL, "/")) != NULL) {
		if (strcmp(value, "github.com") == 0) {
			free(url);
			free(redmine);
			free(bitbucket);
			return github;
		}
		else if (strcmp(value, "bitbucket.org") == 0) {
			free(url);
			free(redmine);
			free(github);
			return bitbucket;
		}
		else if (strcmp(value, "mcs.redmine") == 0) {
			free(url);
			free(github);
			free(bitbucket);
			return redmine;
		}
	}

	strcpy(url, origin_url);

	value = strtok(url, "@");
	while ((value = strtok(NULL, "@")) != NULL) {
		if (strstr(value, "mcs.redmine") != NULL) {
			free(url);
			free(github);
			free(bitbucket);
			return redmine;
		}
	}

	free(url);
	free(github);
	free(redmine);
	free(bitbucket);

	return NULL;
}

