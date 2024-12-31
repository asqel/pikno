#include <stdio.h>
#include <profan/panda.h>
#include <profan.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "win.h"
#include <time.h>
#include <profan/syscall.h>

static void *old_screen;

static void restore_screen(void) {
	panda_screen_restore(old_screen);
	panda_screen_free(old_screen);
}

void write_num_to_win(window_t *win, int x, int y, int num) {
	memset(win->screen, 0, win->_width * win->_height * sizeof(uint16_t));
	char num_str[17] = {0};
	snprintf(num_str, 16, "%d", num);
	for (int i = 0; num_str[i] != '\0'; i++) {
		WIN_CHR_AT(*win, x + i, y) = num_str[i];
		WIN_COL_AT(*win, x + i, y) = 0x0f;
	}
}

#define FPS 60

char *workspace_dir = NULL;

#define SC_EXTEND 224

#define SC_KEY_UP 0x48
#define SC_KEY_DOWN 0x50
#define SC_KEY_LEFT 0x4b
#define SC_KEY_RIGHT 0x4d


int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	old_screen = panda_screen_backup();
	atexit(restore_screen);

	window_t win = {0};
	window_new(&win, 0, 0);
	window_set_size(&win, 20, 20);
	window_move(&win, 10, 10);
	while (1) {
		uint32_t t0 = syscall_timer_get_ms();

		int key = syscall_sc_get();
		if (key == SC_EXTEND) {
			key = syscall_sc_get();
			if (key == SC_KEY_UP) {
				window_move(&win, win._x, win._y - 1);
			} else if (key == SC_KEY_DOWN) {
				window_move(&win, win._x, win._y + 1);
			} else if (key == SC_KEY_LEFT) {
				window_move(&win, win._x - 1, win._y);
			} else if (key == SC_KEY_RIGHT) {
				window_move(&win, win._x + 1, win._y);
			}
		}

		window_render_array(&win, 1, ' ', 0x0f);

		uint32_t t1 = syscall_timer_get_ms();
		if (t1 - t0 < 1000.0 / FPS) {
			if (1000.0 / FPS - (t1 - t0) > 0)
				usleep((1000.0 / FPS - (t1 - t0)) * 1000);
		}
		write_num_to_win(&win, 0, 1, (1000.0 / (syscall_timer_get_ms() - t0)));
	}
	return 0;
}

