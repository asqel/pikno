#include "win.h"
#include <stdlib.h>
#include <string.h>

window_t new_window(int x, int y, int width, int height, wchar_t *text) {
	window_t win = {0};
	win.x = x;
	win.y = y;
	win.width = width;
	win.height = height;

	win.text = new_text(text);
	if (win.text->text[0] == NULL) {
		win.text->text[0] = calloc(sizeof(char_t), 2);
		win.text->text[0][0].ch = L'\n';
		win.text->text[0][1].ch = L'\0';
	}

	return win;
}


static int count_lines(wchar_t *text) {
	int res = 1;
	int len = wcslen(text);
	for (int i = 0; i < len; i++)
		if (L'\n' == text[i])
			res++;
	return res;
}

static int get_line_len(wchar_t *line) {
	int res = 0;
	while (L'\n' != line[res] && L'\0' != line[res])
		res++;
	return res;
}

text_t *new_text(wchar_t *text) {
	text_t *res = {0};

	res = calloc(sizeof(text_t), 1);
	if (NULL == res)
		return NULL;

	res->text_height = count_lines(text);
	res->text = calloc(sizeof(char_t *), res->text_height);
	if (NULL == res->text) {
		free(res);
		return NULL;
	}

	int i = 0;
	int current_line = 0;
	while (L'\0' != text[i]) {
		int line_len = get_line_len(&text[i]);
		res->text[current_line] = calloc(sizeof(char_t), (line_len + 2));
		if (NULL == res->text[current_line]) {
			for (int k = 0; k < current_line; k++)
				free(res->text[k]);
			free(res->text);
			free(res);
			return NULL;
		}
		for (int k = 0; k < line_len; k++) {
			res->text[current_line][k] = (char_t){0};
			res->text[current_line][k].ch = text[i + k];
		}
		res->text[current_line][line_len].ch = L'\n';
		res->text[current_line][line_len + 1].ch = L'\0';
		i += line_len + 1;
		current_line++;
		if (L'\0' == text[i] && current_line < res->text_height) {
			res->text[current_line] = calloc(sizeof(char_t), 2);
			if (NULL == res->text[current_line]) {
				for (int k = 0; k < current_line; k++)
					free(res->text[k]);
				free(res->text);
				free(res);
				return NULL;
			}
			res->text[current_line][0].ch = L'\n';
			res->text[current_line][1].ch = L'\0';
			current_line++;
		}
	}
	return res;
}

void free_text(text_t *text) {
	for (int i = 0; i < text->text_height; i++)
		free(text->text[i]);
	free(text->text);
}

void free_window(window_t *win) {
	free_text(win->text);
	free(win->text);
}