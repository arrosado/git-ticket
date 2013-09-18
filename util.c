#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"

char *getGitEditor();
void writeTemplate(char *fileName, char *string);
char *readFile(char *fileName);
void callEditor(char *editor, char *fileName);

char *getGitEditor() {
	system("git var GIT_EDITOR > in.giteditor");

	char read = 'r';
	char *buffer = NULL;
	FILE *file;

	file = fopen("in.giteditor", &read);

	assert(file != NULL);

	buffer = (char*)malloc(sizeof(char) * 255);

	assert(fgets(buffer, sizeof buffer, file) != NULL);

	fclose(file);

	return buffer;	
}

void writeTemplate(char *fileName, char *string) {

	char write = 'w';
	FILE *file;

	file = fopen(fileName, &write);

	assert(file != NULL);

	fputs(string, file);

	fflush(file);

	fclose(file);

	return;
}

char *readFile(char *fileName) {
	char read = 'r';
	char *buffer = NULL;
	unsigned long lSize;
	size_t result;
	FILE *file;

	file = fopen(fileName, &read);

	assert(file != NULL);

	// Obtain file size.
	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);

	buffer = (char*)malloc(sizeof(char)*lSize);

	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	result = fread(buffer, 1, lSize, file);

	if (result != lSize) { fputs("Reading error", stderr); exit(3); }

	fclose(file);

	return buffer;
}

void callEditor(char *editor, char *fileName) {
	char *cmd = NULL;

	cmd = (char*)malloc(sizeof(char) * (strlen(editor) + strlen(fileName) + 1));

	sprintf(cmd, "%s %s", editor, fileName);

	system(cmd);

	free(cmd);
}

char *inputWithEditor(char *string) {

	char path[11] = "in.editing";
	char *temp = NULL;
	char *editor = NULL;	

	temp = (char*)malloc(sizeof(char) *(strlen(path) + 1));

	strcpy(temp, path);

	editor = getGitEditor();

	writeTemplate(temp, string);

	callEditor(editor, temp);

	free(editor);

	return readFile(temp);
}

