
#include "win.h"

int cursor_screen_x = -1;
int cursor_screen_y = -1;

#if defined(USE_NCURSES)
	#include <ncurses.h>
	#include <locale.h>
	#define SET_CHR(y, x, ch) do {wchar_t __t_m_p_[2] = {ch, L'\0'};mvaddwstr(y, x, __t_m_p_);} while(0)
	#define SET_COL(fg, bg) attron(COLOR_PAIR(fg << 4 | bg))
	#define ENABLE_BOLD() attron(A_BOLD)
	#define DISABLE_BOLD() attroff(A_BOLD)
	#define ENABLE_UNDERLINE() attron(A_UNDERLINE)
	#define DISABLE_UNDERLINE() attroff(A_UNDERLINE)
	#define ENABLE_REVERSE() attron(A_REVERSE)
	#define DISABLE_REVERSE() attroff(A_REVERSE)
	#define ENABLE_BLINK() attron(A_BLINK)
	#define DISABLE_BLINK() attroff(A_BLINK)
	#define ENABLE_ITALIC() attron(A_ITALIC)
	#define DISABLE_ITALIC() attroff(A_ITALIC)

	#define DISABLE_CURSOR() curs_set(FALSE)
	#define ENABLE_CURSOR() curs_set(TRUE)
	#define SET_CURSOR_POS(y, x) move(y, x)
	#define CLEAR_SCREEN() erase()
#else
	#error "Unsupported platform in file display.c"
#endif

void display_init() {
	#if defined(USE_NCURSES)
		setlocale(LC_ALL, "");
		initscr();
		noecho();
		scrollok(stdscr, FALSE);
		curs_set(TRUE);
		timeout(500);
		// init color pairs
		start_color();
		for (int fg = 0; fg < 9; fg++)
			for (int bg = 0; bg < 9; bg++)
				init_pair(fg << 4 | bg, fg, bg);

	#else
		#error "Unsupported platform in file display.c"
	#endif
}

static void display_window(window_t win) {
	uint32_t *win_screen = calloc(sizeof(uint32_t), (win.height * win.width));


	int screen_y = 0;
	int text_y = win.view_y;
	while (screen_y < win.height && text_y < win.text->text_height) {
		if (text_y < 0) {
			text_y++;
			screen_y++;
			continue;
		}
		int text_x = win.view_x;
		int screen_x = 0;
		int line_len = 0;
		while (win.text->text[text_y][line_len].ch != '\0')
			line_len++;
		if (line_len == 0 && text_y == win.cursor_y) {
			cursor_screen_x = screen_x + win.x;
			cursor_screen_y = screen_y + win.y;
		}
		while (screen_x < win.width && text_x < line_len) {
			if (text_x < 0) {
				text_x++;
				screen_x++;
				continue;
			}
			if (text_y == win.cursor_y && text_x == win.cursor_x) {
				cursor_screen_x = screen_x + win.x;
				cursor_screen_y = screen_y + win.y;
			}
			if (win.text->text[text_y][text_x].ch != '\t') {
				win_screen[screen_y * win.width + screen_x] = win.text->text[text_y][text_x].ch;
				screen_x++;
			}
			else {
				win_screen[screen_y * win.width + screen_x] = ' ';
				screen_x += (4 - (text_x % 4));
			}
			text_x++;
		}
		text_y++;
		screen_y++;
	}
	for (int y = 0; y < win.height; y++) {
		for (int x = 0; x < win.width; x++) {
			SET_CHR(y + win.y, x + win.x, win_screen[x + y *win.width]);
			///unsigned char c = win_screen[x + y * win.width];
			///fprintf(stderr, "%c%c ", "0123456789ABCDEF"[c >> 8], "0123456789ABCDEF"[c & 0xFF]);
		}
	}
	free(win_screen);

}

void display_wins(window_t *wins, int len) {
	cursor_screen_x = -1;
	cursor_screen_y = -1;
	DISABLE_CURSOR();
	CLEAR_SCREEN();
	for (int i = 0; i < len; i++) {
		display_window(wins[i]);
	}
}

void display_update() {
	#if defined(USE_NCURSES)
		if (cursor_screen_x != -1 && cursor_screen_y != -1)
		{
			SET_CURSOR_POS(cursor_screen_y, cursor_screen_x);
			ENABLE_CURSOR();
		}
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
