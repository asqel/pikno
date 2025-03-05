#include <stdio.h>
#include <stdlib.h>
#include "win.h"
#include "file.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

static int is_binary(wchar_t c) {
	return c < 0x09 || (0x0D <= c && c < 0x20) || c == 0x7F;
}
// remove \r
wchar_t *fix_file(wchar_t *content) {
	wchar_t *res = malloc(sizeof(wchar_t) * (wcslen(content) + 1));
	if (res == NULL) {
		return NULL;
	}
	int j = 0;
	for (int i = 0; content[i] != L'\0'; i++) {
		if (!is_binary(content[i])) {
			res[j] = content[i];
			j++;
		}
	}
	res[j] = L'\0';
	wchar_t *tmp = realloc(res, sizeof(wchar_t) * (wcslen(res) + 1));
	if (tmp == NULL) {
		free(res);
		return NULL;
	}
	return tmp;
}

char *read_file_char(char *path) {
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	char *buffer = malloc(size + 1);
	if (NULL == buffer) {
		fclose(f);
		return NULL;
	}
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, size, f);
	fclose(f);
	buffer[size] = 0;
	return buffer;
}


wchar_t *read_file(char *path) {
	setlocale(LC_ALL, "");

    char *content = read_file_char(path);
    if (content == NULL)
        return NULL;

    size_t len = strlen(content);
    wchar_t *res = malloc((len + 1) * sizeof(wchar_t));
    if (res == NULL) {
        free(content);
        return NULL;
    }

    size_t converted = mbstowcs(res, content, len);
    if (converted == (size_t)-1) {
        free(content);
        free(res);
        return NULL;
    }

    res[converted] = L'\0';
    free(content);
    return fix_file(res);
}
