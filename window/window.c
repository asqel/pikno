#include <stdint.h>
#include <stdlib.h>
#include <profan/panda.h>
#include "../win.h"
#include <profan.h>

void window_new(window_t *window, int x, int y) {
	window->_x = x;
	window->_y = y;
	window->_width = 0;
	window->_height = 0;
	window->screen = NULL;
	window->draw = NULL;
	window->destroy_data = NULL;
	window->tick = NULL;
	window->on_event = NULL;
	window->_actions = NULL;
	window->_action_alloc_size = 0;
	window->_action_count = 0;
	window->data = NULL;
}

// setting either width or height to 0 lead to freeing the screen and setting both to 0
// window cannot have a size that would collide oustide the screen
//     if so the size will be the maximum width or height depending on which is outside
// if width or height is less than 0 nothing will be done
void window_set_size(window_t *window, int width, int height) {
	if (width == window->_width && height == window->_height)
		return ;
	if (width < 0 || height < 0)
	if (width == 0 || height == 0) {
		free(window->screen);
		window->_width = 0;
		window->_height = 0;
		return ;
	}

	int scr_h = 0;
	int scr_w = 0;
	panda_get_size((uint32_t *)&scr_w, (uint32_t *)&scr_h);
	if (window->_x + width > scr_w)
		width = scr_w - window->_x;
	if (window->_y + window->_height > scr_h)
		height = scr_h - window->_y;

	uint16_t *new_buff = calloc(height * width, sizeof(uint16_t));
	int min_width = width;
	int min_height = height;

	if (window->_width < min_width)
		min_width = window->_width;
	if (window->_height < min_height)
		min_height = window->_height;

	for (int x = 0; x < min_width; x++)
		for (int y = 0; y < min_height; y++)
			new_buff[(x + y * width)] = WIN_CHR_AT(*window, x, y);
	free(window->screen);
	window->screen = new_buff;
	window->_height = height;
	window->_width = width;
}

// return 1 if has moved to the desired position, 0 otherwise
// if x or y is less than 0, the window will not move
// if the window would collide outside the screen, it will be moved to the maximum position
int window_move(window_t *window, int x, int y) {
	if (x < 0 || y < 0)
		return 0;
	if (window->_x == x && window->_y == y)
		return 0;
	// check if collide outside screen
	int screen_width = 0;
	int screen_height = 0;
	panda_get_size((uint32_t *)&screen_width, (uint32_t *)&screen_height);
	if (x + window->_width > screen_width)
		window->_x = screen_width - window->_width;
	else
		window->_x = x;
	if (y + window->_height > screen_height)
		window->_y = screen_height - window->_height;
	else
		window->_y = y;
	return x == window->_x && y == window->_y;
}

action_t *window_get_oldest_action(window_t *window) {
	if (window->_action_count == 0) {
		return NULL;
	}
	action_t *action = &window->_actions[0];
	for (int i = 1; i < window->_action_count; i++) {
		window->_actions[i - 1] = window->_actions[i];
	}
	window->_action_count--;
	// if the gap between the number of actions and the allocated size is too big, shrink the array
	if (window->_action_count < window->_action_alloc_size - 10) {
		window->_actions = realloc(window->_actions, window->_action_count * sizeof(action_t));
		window->_action_alloc_size = window->_action_count;
	}
	return action;
}

void window_add_action(window_t *window, action_t action) {
	if (window->_action_count == window->_action_alloc_size) {
		window->_action_alloc_size += 10;
		window->_actions = realloc(window->_actions, window->_action_alloc_size * sizeof(action_t));
	}
	window->_actions[window->_action_count] = action;
	window->_action_count++;
}

void window_destroy(window_t *window) {
	if (window->destroy_data != NULL) {
		window->destroy_data(window);
	}
	if (window->_actions != NULL) {
		free(window->_actions);
	}
}

void window_render(window_t *win) {
	for (int i = 0; i < win->_width; i++)
		for (int k = 0; k < win->_height; k++) {
			if (WIN_CHR_AT(*win, i, k) != '\0')
				panda_set_char(i + win->_x, k + win->_y, WIN_CHR_AT(*win, i, k), WIN_COL_AT(*win, i, k));
		}
}

void window_render_array(window_t *windows, int len, char c, uint8_t col) {
	int scr_w = 0;
	int scr_h = 0;
	panda_get_size((uint32_t *)&scr_w, (uint32_t *)&scr_h);
	for (int x = 0; x < scr_w; x++)
		for (int y = 0; y < scr_h; y++)
			panda_set_char(x, y, c, col);

	for (int i = 0; i < len; i++) {
		if (windows[i].draw != NULL) {
			windows[i].draw(&windows[i]);
		}
	}
	for (int i = 0; i < len; i++) {
		window_render(&(windows[i]));
	}
}

