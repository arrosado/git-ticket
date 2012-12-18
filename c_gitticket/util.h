#ifndef __UTIL_H__
#define __UTIL_H__

int strwidth(char* str);
void cmd_stdout(char** argv);
void rmcomment(char* text);
void regex_extract(char* pattern, char* target, char* def);
char* inputwitheditor(char* str);
void memoize(void* obj);

#endif