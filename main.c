#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"

int main(int argc, char** argv) { 

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

	git_dispose();
	
	return 0;
}
