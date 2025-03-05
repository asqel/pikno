#ifndef WIN_H
#define WIN_H

#include <stdint.h>
#include <wchar.h>

typedef struct {
	uint32_t color;
	uint32_t bgcolor;
	uint32_t ch; // character
} char_t;

typedef struct {
	char_t **text;
	int text_height;
} text_t;

typedef struct {
	int x;
	int y;
	int width;
	int height;

	text_t *text;
	int cursor_x;
	int cursor_y;
	int view_x;
	int view_y;


} window_t;

window_t new_window(int x, int y, int width, int height, wchar_t *text);
text_t *new_text(wchar_t *text);
void display_wins(window_t *win, int len);
void display_init();
void display_update();
void display_exit();

#endif