#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"

int main(int argc, char** argv) { 
	printf("git-ticket working correctly.");
	
	Git *git;

	git_initialize(git);

	git_getConfig(git, "user.name");
	//if (git_getConfig(git, "user.name"))
	//	printf("Username is %s", git_getConfig(git, "user.name"));

	git_dispose(git);
	
	return 0;
}
