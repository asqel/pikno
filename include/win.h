#ifndef WIN_H
#define WIN_H

#include <stdint.h>

typedef struct {
	uint16_t color;
	uint16_t bgcolor;
	uint16_t ch; // character
} char_t;

typedef struct {
	int x;
	int y;
	int width;
	int height;

	char_t **text;
	int text_height;
	int cursor_x;
	int cursor_y;
	int view_x;
	int view_y;


} window_t;

char **strsplit(char *str, char delim, int *count);
window_t new_window(int x, int y, int width, int height, char *text);

#endif