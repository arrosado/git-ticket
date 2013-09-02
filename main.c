#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

struct Node
{
	char *key;
	char *value;
	struct Node *next;
};

struct Node *list = NULL, **nextp = &list;
char line[1024];

int main(int argc, char** argv) { 
	printf("git-ticket working correctly.");
	system("git config -l > in.list");
	
	FILE *ifp;
	char mode = 'r';

	ifp = fopen("in.list", &mode);

	if (ifp == NULL) {
		fprintf(stderr, "Can't open input file  in.list!\n");
		exit(1);
	}

	while (fgets(line, sizeof line, ifp) != NULL) {
			
		struct Node *node;
		node = (Node *)malloc(sizeof(struct Node) + strlen(line) + 1);
		node->key = strtok(strcpy((char*)(node+1), line), "=\r\n");
		node->value = strtok(NULL, "\r\n");
		node->next = NULL;
		*nextp = node;
		nextp = &node->next;
	}

	Node * item;

	while ((item = list->next) != NULL) {
		printf("%s is equal to %s\n", item->key, item->value);
	}


	fclose(ifp);

}