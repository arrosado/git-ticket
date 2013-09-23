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

struct git_ticket_config *git_parseConfig(int doVerify) {
	struct git_ticket_config *config = (struct git_ticket_config*)malloc(sizeof(struct git_ticket_config));
	
	const char *repo_path = git_dir();
	char config_path[256];
	git_config *local_cfg;
	git_config *default_cfg;

	sprintf(config_path, "%s/config", repo_path);

	check_error(git_config_open_ondisk(&local_cfg, config_path), "opening local config file");
	check_error(git_config_open_default(&default_cfg), "opening default config file");

	git_config_get_string(&config->name, local_cfg, "ticket.name");
	if (!config->name)
		git_config_get_string(&config->name, default_cfg, "user.name");

	git_config_get_string(&config->repo, local_cfg, "ticket.repo");
	if (!config->repo) 
		config->repo = git_guess_repo_name();

	git_config_get_string(&config->service, local_cfg, "ticket.service");
	if (!config->service)
		config->service = git_guess_service();

	const char *httpssl;
	git_config_get_string(&httpssl, local_cfg, "ticket.ssl");

	if (httpssl)
		config->ssl = strcmp(httpssl, "true") == 0;

	git_config_get_string(&config->format_list, local_cfg, "ticket.format.list");
	git_config_get_string(&config->format_show, local_cfg, "ticket.format.show");
	git_config_get_string(&config->format_comment, local_cfg, "ticket.format.comment");
	git_config_get_string(&config->gtoken, local_cfg, "ticket.github.token");
	git_config_get_string(&config->btoken, local_cfg, "ticket.bitbucket.token");
	git_config_get_string(&config->btoken_secret, local_cfg, "ticket.bitbucket.token-secret");
	git_config_get_string(&config->rurl, local_cfg, "ticket.redmine.url");
	git_config_get_string(&config->rpassword, local_cfg, "ticket.redmine.password");
	git_config_get_string(&config->rtoken, local_cfg, "ticket.redmine.token");

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

int isurl(char *url) {
	return (strstr(url, "://") != NULL);
}

char *git_guess_repo_name() {
	const char *origin_url;
	const char *repo_path = git_dir();
	char config_path[256];
	git_config *local_cfg;
	git_config *default_cfg;

	sprintf(config_path, "%s/config", repo_path);

	check_error(git_config_open_ondisk(&local_cfg, config_path), "opening local config file");
	check_error(git_config_open_default(&default_cfg), "opening default config file");

	git_config_get_string(&origin_url, local_cfg, "remote.origin.url");

	if (!origin_url)
		return NULL;
printf("\n\n\nHere\n\n\n");
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
	const char *origin_url;
	const char *repo_path = git_dir();
	char config_path[256];
	git_config *local_cfg;
	git_config *default_cfg;

	sprintf(config_path, "%s/config", repo_path);

	check_error(git_config_open_ondisk(&local_cfg, config_path), "opening local config file");
	check_error(git_config_open_default(&default_cfg), "opening default config file");

	git_config_get_string(&origin_url, local_cfg, "remote.origin.url");

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

