#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>

static void check_error(int error_code, const char *action)
{
	if (!error_code)
		return;

	const git_error *error = giterr_last();

	printf("Error %d %s - %s\n", error_code, action, 
		(error && error->message) ? error->message : "???");

	exit(1);
}

int main(void) { 

	printf("\n*** Configuration Listing ***\n");

	const char *repo_path = "./.git";
	const char *email;
	const char *url;
	char config_path[256];
	git_config *cfg;
	git_repository *repo;
	int32_t j;

	check_error(git_repository_open(&repo, repo_path), "opening repository");
	
	sprintf(config_path, "%s/config", repo_path);
	check_error(git_config_open_ondisk(&cfg, config_path), "opening config");
	//check_error(git_config_open_default(&cfg), "opening config");
	//check_error(git_config_open_global(&cfg), "opening config");


	git_config_get_int32(&j, cfg, "ticket.ssl");
	printf("Autocorrect: %d\n", j);

	git_config_get_string(&url, cfg, "ticket.redmine.password");
	printf("Password: %s\n", url);

	git_config_get_string(&email, cfg, "user.email");
	printf("Email: %s\n", email);

	git_repository_free(repo);

	return 0;
}
