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
		printf("Is in working tree\n\n");	

	git_dispose();
	
	return 0;
}
