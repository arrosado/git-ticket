#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <git2.h>
#include "config.h"

int string_indexof(char *str, char find) {
	return strchr(str, find) - str;
}

static void check_error(int error_code, const char *action)
{
	if (!error_code)
		return;

	const git_error *error = giterr_last();

	printf("Error %d %s - %s\n", error_code, action, 
		(error && error->message) ? error->message : "???");

	exit(1);
}

int git_isInsideWorkTree() {
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

char **git_branches() {
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

void 
get_config(const char **string, const char *key, const char * altkey)
{
	const char *string_empty = "";
	const char *repo_path = git_dir();
	char config_path[256];
	git_config *local_cfg;
	git_config *default_cfg;

	sprintf(config_path, "%s/config", repo_path);

	check_error(git_config_open_ondisk(&local_cfg, config_path), "opening local config file");
	check_error(git_config_open_default(&default_cfg), "opening default config file");

	if(git_config_get_string(string, local_cfg, key))
		if (!altkey || git_config_get_string(string, default_cfg, altkey))
			*string = string_empty;
	
	if (strcmp(key, "ticket.repo") == 0 && strcmp(*string, string_empty) == 0)
		*string = git_guess_repo_name();

	if (strcmp(key, "ticket.service") == 0 && strcmp(*string, string_empty) == 0)
		*string = git_guess_service();

	return;
}

struct git_ticket_config 
*git_parseConfig(int doVerify) {
	struct git_ticket_config *config = (struct git_ticket_config*)malloc(sizeof(struct git_ticket_config));

	get_config(&config->name, "ticket.name", "user.name");
	get_config(&config->repo, "ticket.repo", NULL);
	get_config(&config->service, "ticket.service", NULL);
	const char *httpssl;
	get_config(&httpssl, "ticket.ssl", NULL);
	if (httpssl) 
		config->ssl = strcmp(httpssl, "true") == 0;
	get_config(&config->format_list, "ticket.format.list", NULL);
	get_config(&config->format_show, "ticket.format.show", NULL);
	get_config(&config->format_comment, "ticket.format.comment", NULL);
	get_config(&config->gtoken, "ticket.github.token", NULL);
	get_config(&config->btoken, "ticket.bitbucket.token", NULL);
	get_config(&config->btoken_secret, "ticket.bitbucket.token-secret", NULL);
	get_config(&config->rurl, "ticket.redmine.url", NULL);
	get_config(&config->rpassword, "ticket.redmine.password", NULL);
	get_config(&config->rtoken, "ticket.redmine.token", NULL);


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

		if (config->name == NULL || strcmp(config->name, "") == 0) {
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

int isurl(char *url) {
	return (strstr(url, "://") != NULL);
}

char *git_guess_repo_name() {
	const char *origin_url;

	get_config(&origin_url, "remote.origin.url", NULL);

	if (!origin_url)
		return NULL;

	char *url = (char*)malloc(sizeof(char) * (strlen(origin_url) + 1));

	strcpy(url, origin_url);

	if (!url) 
		return NULL;

	char *repo_name = NULL;
	char *temp_name = strtok(url, "/");

	while ((temp_name = strtok(NULL, "/")) != NULL) {
		free(repo_name);
		repo_name = (char*)malloc(sizeof(char) * (strlen(temp_name) + 1));
		strcpy(repo_name, temp_name);
	}

	if (repo_name != NULL){
		free(url);
		free(temp_name);
		return repo_name;
	}

	temp_name = strtok(url, ":");

	while ((temp_name = strtok(NULL, ":")) != NULL) {
		free(repo_name);
		repo_name = (char*)malloc(sizeof(char) * (strlen(temp_name) + 1));
		strcpy(repo_name, temp_name);
	}

	free(temp_name);
	free(url);

	return repo_name;
}

char *git_guess_service() {
	const char *origin_url;

	get_config(&origin_url, "remote.origin.url", NULL);

	char *url = (char*)malloc(sizeof(char) * (strlen(origin_url) + 1));

	strcpy(url, origin_url);

	if (!url) 
		return NULL;

	char *github = (char*)malloc(sizeof(char) * (strlen("github") + 1));
	strcpy(github, "github");

	char *bitbucket = (char*)malloc(sizeof(char) * (strlen("bitbucket") + 1));
	strcpy(bitbucket, "bitbucket");

	char *redmine = (char*)malloc(sizeof(char) * (strlen("redmine") + 1));
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

