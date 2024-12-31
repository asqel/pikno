#ifndef WIN_H
#define WIN_H

#include <stdint.h>
#include <stdlib.h>
#include <profan/panda.h>

#define WIN_CHR_AT(SELF, X, Y) (((char *)(SELF).screen)[((X) + (Y) * (SELF)._width) * sizeof(uint16_t)])
#define WIN_COL_AT(SELF, X, Y) (((unsigned char *)(SELF).screen)[((X) + (Y) * (SELF)._width) * sizeof(uint16_t) + 1])

typedef struct {
	uint8_t type;
	union {
		char key_text[5];
		uint8_t event;
	} val;
} action_t;

#define COL_WHITE 0x0f

// member starting with '_' are private to the window engine (they can still be read)
// data is a user defined and is free by the destroy data function
// destroy data function is called when the window is destroyed
typedef struct window_t{
	int _x;
	int _y;
	int _width;
	int _height;
	uint16_t *screen; // char, color
	void (*draw)(struct window_t *self);
	void (*destroy_data)(struct window_t *self);
	void (*tick)(struct window_t *self);
	void (*on_event)(struct window_t *self, uint8_t event, ...);
	action_t *_actions;
	int _action_count;
	int _action_alloc_size;
	void *data;
	uint8_t has_moved; // 1 when moved or resized
	int _last_x; // last since draw to screen
	int _last_y;
	int _last_h;
	int _last_w;
} window_t;

void window_new(window_t *window, int x, int y);

// setting either width or height to 0 lead to freeing the screen and setting both to 0
// window cannot have a size that would collide oustide the screen
//     if so the size will be the maximum width or height depending on which is outside
// if width or height is less than 0 nothing will be done
void window_set_size(window_t *window, int width, int height);

// return 1 if has moved to the desired position, 0 otherwise
// if x or y is less than 0, the window will not move
// if the window would collide outside the screen, it will be moved to the maximum position
int window_move(window_t *window, int x, int y);
action_t *window_get_oldest_action(window_t *window);
void window_add_action(window_t *window, action_t action);
void window_destroy(window_t *window);
void window_render(window_t *win);
void window_render_array(window_t *windows, int len, char c, uint8_t col);

#endif