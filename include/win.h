#ifndef WIN_H
#define WIN_H

#include <stdint.h>
#include <wchar.h>

typedef struct {
	uint32_t ch; // character

	uint8_t fg:4; // foreground color
	uint8_t bg:4; // background color

	uint8_t bold:1;
	uint8_t underline:1;
	uint8_t reverse:1;
	uint8_t blink:1;
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
	int is_focused;

} window_t;

window_t new_window(int x, int y, int width, int height, wchar_t *text);
text_t *new_text(wchar_t *text);
void display_wins(window_t *win, int len);
void display_init();
void display_update();
void display_exit();

#if defined(USE_NCURSES)
	#define P_COL_BLACK ((int)COLOR_BLACK)
	#define P_COL_RED ((int)COLOR_RED)
	#define P_COL_GREEN ((int)COLOR_GREEN)
	#define P_COL_BLUE ((int)COLOR_BLUE)
	#define P_COL_YELLOW ((int)COLOR_YELLOW)
	#define P_COL_MAGENTA ((int)COLOR_MAGENTA)
	#define P_COL_CYAN ((int)COLOR_CYAN)
	#define P_COL_WHITE ((int)COLOR_WHITE)
#else
	#error "Unspported platform in file win.h"
#endif

#endif