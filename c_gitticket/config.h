#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef struct {
       char** keys;
       char** values;
} configuration;

void git();
int is_inside_work_tree();
char* gitdir();
configuration* parseconfig();
void verify(configuration* config);
char* guess_repo_name();
void guess_service();
int isurl(char* url);
char* originalurl(char* url);
int nested_access(char* dest, char* keystr, char* def);
char* conftodict(configuration* config);

#endif 