#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int main(void) { 

	printf("Hello world!\n");

	char *temp = NULL;
	char cmd[255] = "[subject]\n\n[body]\n\n[refs]\n\n";

	temp = (char*)malloc(sizeof(char) * strlen(cmd) + 1);

	strcpy(temp, cmd);

	printf("%s\n", inputWithEditor(temp));

	free(temp);

	return 0;
}
