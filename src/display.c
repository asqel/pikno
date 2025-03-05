
#include "win.h"


#if defined(USE_NCURSES)
	#include <ncurses.h>
	#include <locale.h>
	#define SET_CHR(y, x, ch) do {wchar_t tmp[2] = {ch, L'\0'};mvaddwstr(y, x, tmp);} while(0)
#else
	#error "Unsupported platform in file display.c"
#endif

void display_init() {
	#if defined(USE_NCURSES)
		setlocale(LC_ALL, "");
		initscr();
		noecho();
		curs_set(FALSE);
	#else
		#error "Unsupported platform in file display.c"
	#endif
}

static void display_window(window_t win) {
	// draw border
	for (int i = 0; i < win.height + 2; i++) {
		int y = win.y + i - 1;
		SET_CHR(y, win.x - 1, '|');
		SET_CHR(y, win.x + win.width, '|');
	}

	for (int i = 0; i < win.width + 2; i++) {
		int x = win.x + i - 1;
		SET_CHR(win.y - 1, x, '-');
		SET_CHR(win.y + win.height, x, '-');
	}

	// draw + in corners
	SET_CHR(win.y - 1, win.x - 1, '+');
	SET_CHR(win.y - 1, win.x + win.width, '+');
	SET_CHR(win.y + win.height, win.x - 1, '+');
	SET_CHR(win.y + win.height, win.x + win.width, '+');

	int real_y = win.view_y;
	while (real_y - win.view_y < win.height && real_y < win.text->text_height) {
		if (real_y < 0) {
			real_y++;
			continue;
		}
		int real_x = win.view_x;
		int line_len = 0;
		while (win.text->text[real_y][line_len].ch != '\0')
			line_len++;
		while (real_x - win.view_x < win.width && real_x < line_len) {
			if (real_x < 0) {
				real_x++;
				continue;
			}
			SET_CHR(
				win.y + real_y - win.view_y,
				win.x + real_x - win.view_x,
				win.text->text[real_y][real_x].ch
			);

			real_x++;
		}
		real_y++;
	}

}

void display_wins(window_t *wins, int len) {
	#if defined(USE_NCURSES)
		clear();
	#else
		#error "Unsupported platform in file display.c"
	#endif

	for (int i = 0; i < len; i++) {
		display_window(wins[i]);
	}
}

void display_update() {
	#if defined(USE_NCURSES)
		refresh();
	#else
		#error "Unsupported platform in file display.c"
	#endif
}

void display_exit() {
	#if defined(USE_NCURSES)
		endwin();
	#else
		#error "Unsupported platform in file display.c"
	#endif
}
