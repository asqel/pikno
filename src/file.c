#include <stdio.h>
#include <stdlib.h>

// remove \r
char *fix_file(char *content) {
	char *res = malloc(strlen(content) + 1);
	if (res == NULL) {
		return NULL;
	}
	int j = 0;
	for (int i = 0; i < strlen(content); i++) {
		if (content[i] != '\r') {
			res[j] = content[i];
			j++;
		}
	}
	res[j] = 0;
	return realloc(res, j + 1);
}

char *read_file(char *path) {
	FILE *f = fopen(path, "r");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	char *buffer = malloc(size + 1);
	if (buffer == NULL) {
		fclose(f);
		return NULL;
	}
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, size, f);
	fclose(f);
	buffer[size] = 0;
	return fix_file(buffer);
}