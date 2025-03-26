#include "win.h"

void cursor_move_up(window_t *win) {
	if (win->cursor_y == 0) {
		win->cursor_x = 0;
		if (win->view_x > 0)
			win->view_x = 0;
		return ;
	}
	win->cursor_y--;
	if (win->view_y > win->cursor_y)
		win->view_y = win->cursor_y;
	int real_x = win->cursor_x;
	int line_len = 0;
	while (win->text->text[win->cursor_y][line_len].ch != '\0')
		line_len++;
	if (real_x >= line_len)
		win->cursor_x = line_len - 1;
	if (win->cursor_x < 0)
		win->cursor_x = 0;
	if (win->cursor_x < win->view_x)
		win->view_x = win->cursor_x;
	if (win->cursor_x >= win->view_x + win->width)
		win->view_x = win->cursor_x - win->width;
}

void cursor_move_down(window_t *win) {
	if (win->cursor_y == win->text->text_height - 1) {
		int line_len = 0;
		while (win->text->text[win->cursor_y][line_len].ch != '\0')
			line_len++;
		win->cursor_x = line_len;
		if (win->cursor_x < 0)
			win->cursor_x = 0;
		if (win->view_x > win->cursor_x)
			win->view_x = win->cursor_x;
		if (win->view_x + win->width < win->cursor_x)
			win->view_x = win->cursor_x - win->width;
		return ;
	}
	win->cursor_y++;
	if (win->cursor_y >= win->view_y + win->height)
		win->view_y++;
	int real_x = win->cursor_x;
	int line_len = 0;
	while (win->text->text[win->cursor_y][line_len].ch != '\0')
		line_len++;
	if (real_x >= line_len)
		win->cursor_x = line_len - 1;
	if (win->cursor_x < 0)
		win->cursor_x = 0;
	if (win->cursor_x < win->view_x)
		win->view_x = win->cursor_x;
	if (win->cursor_x >= win->view_x + win->width)
		win->view_x = win->cursor_x - win->width;
}

void cursor_move_right(window_t *win) {
	int line_len = 0;
	while (win->text->text[win->cursor_y][line_len].ch != '\0')
		line_len++;
	if (win->cursor_x == line_len - 1 && win->cursor_y == win->text->text_height - 1)
		return ;
	if (win->cursor_x >= line_len - 1) {
		cursor_move_down(win);
		win->cursor_x = 0;
		if (win->view_x > 0)
			win->view_x = 0;
		return ;
	}
	win->cursor_x++;
	if (win->cursor_x >= win->view_x + win->width)
		win->view_x++;
}

void cursor_move_left(window_t *win) {
	if (win->cursor_x == 0 && win->cursor_y == 0) {
		return ;
	}
	if (win->cursor_x == 0) {
		cursor_move_up(win);
		int line_len = 0;
		while (win->text->text[win->cursor_y][line_len].ch != '\n')
			line_len++;
		win->cursor_x = line_len;
		if (win->cursor_x < 0)
			win->cursor_x = 0;
		if (win->view_x > win->cursor_x)
			win->view_x = win->cursor_x;
		if (win->view_x + win->width < win->cursor_x)
			win->view_x = win->cursor_x - win->width;
		return ;
	}
	int len = 0;
	while (win->text->text[win->cursor_y][len].ch != '\0')
		len++;
	if (win->cursor_x >= len)
		win->cursor_x = len - 1;
	if (win->cursor_x > 0)
		win->cursor_x--;
	else if (win->cursor_x == 0) {
		cursor_move_up(win);
		int line_len = 0;
		while (win->text->text[win->cursor_y][line_len].ch != '\0')
			line_len++;
		win->cursor_x = line_len - 1;
		if (win->cursor_x < 0)
			win->cursor_x = 0;
		if (win->view_x > win->cursor_x)
			win->view_x = win->cursor_x;
		if (win->view_x + win->width < win->cursor_x)
			win->view_x = win->cursor_x - win->width;
	}
	if (win->cursor_x < win->view_x)
		win->view_x--;
}