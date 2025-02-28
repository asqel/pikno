#include "win.h"
#include <stdlib.h>
#include <string.h>

char **strsplit(char *str, char delim, int *count) {
	*count = 0;
	char *p = strtok(str, &delim);
	char **arr = NULL;
	int i = 0;
	while (p != NULL) {
		*count = *count + 1;
		arr = realloc(arr, sizeof(char *) * (i + 1));
		if (arr == NULL) {
			free(arr);
			return NULL;
		}
		arr[i] = p;
		p = strtok(NULL, &delim);
		i++;
	}
	return arr;
}

window_t new_window(int x, int y, int width, int height, char *text) {
	window_t win = {0};
	win.x = x;
	win.y = y;
	win.width = width;
	win.height = height;


	char **lines = strsplit(text, '\n', &win.text_height);
	if (lines == NULL) {
		return win;
	}

	win.text = malloc(sizeof(char_t *) * win.text_height);
	if (win.text == NULL) {
		free(lines);
		return win;
	}

	for (int i = 0; i < win.text_height; i++) {
		win.text[i] = malloc(sizeof(char_t) * strlen(lines[i]) + 1);
		if (win.text[i] == NULL) {
			free(win.text);
			free(lines);
			return win;
		}
		for (int j = 0; j < strlen(lines[i]); j++) {
			win.text[i][j].color = 0;
			win.text[i][j].bgcolor = 0;
			win.text[i][j].ch = lines[i][j];
		}
		win.text[i][strlen(lines[i])].ch = '\0';
	}
	return win;
}