#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int string_indexof(char *str, char find) {
	return strchr(str, find) - str;
}
