
#include "win.h"


#if defined(USE_NCURSES)
	#include <ncurses.h>

	static int get_key() {
		return getch();
	}
#else
	#error "Unsupported platform in file keys.c"
#endif

int handle_keys(window_t *wins, int len) {
	window_t *window = NULL;
	for (int i = 0; i < len; i++) {
		if (wins[i].is_focused) {
			window = &wins[i];
			break;
		}
	}
	if (window == NULL)
		return 0;
	int key = get_key();
	static FILE *f = NULL;
	if (f == NULL)
		f = fopen("log.txt", "w");
	fprintf(f, "key %d %d\n", key, key == 0x1b);
	fflush(f);
	if (key == 0x1b) {
		key = get_key();
		fprintf(f, "ici %d\n", key);
		fflush(f);
		if (key == '[') {
			key = get_key();
			fprintf(f, "et la %d\n", key);
			fflush(f);
			if (key == 'A')
				cursor_move_up(window);
			if (key == 'B')
				cursor_move_down(window);
			if (key == 'C')
				cursor_move_right(window);
			if (key == 'D')
				cursor_move_left(window);
			if (key == '3') { // del
				key = get_key();
				if (key == '~') {
					// print current line to f
					for (int i = 0; window->text->text[window->cursor_y][i].ch != '\n'; i++)
					{
						unsigned char c = window->text->text[window->cursor_y][i].ch;
						fprintf(f, "%c%c ", "0123456789ABCDEF"[c >> 4] ,"0123456789ABCDEF"[c & 0xf]);
						fflush(f);
					}
					fprintf(f,"\n\t %d\n", window->cursor_x);
					fflush(f);
					if (window->text->text[window->cursor_y][window->cursor_x].ch == '\n')
						return 1;
					int line_len = 0;
					while (window->text->text[window->cursor_y][line_len].ch != '\0')
						line_len++;
					if (line_len <= 1)
						return 1;
					if (line_len == 2) {
						char_t *new_line = malloc(sizeof(char_t) * 2);
						if (new_line == NULL)
							return 0;
						new_line[0] = (char_t){.ch = L'\n', .fg = 0, .bg = 0, .bold = 0, .underline = 0, .reverse = 0, .blink = 0};
						new_line[1] = (char_t){.ch = L'\0', .fg = 0, .bg = 0, .bold = 0, .underline = 0, .reverse = 0, .blink = 0};
						free(window->text->text[window->cursor_y]);
						window->text->text[window->cursor_y] = new_line;
						window->cursor_x = 0;
						return 1;
					}
					char_t *new_line = malloc(sizeof(char_t) * (line_len - 1 + 1));
					int end = 0;
					int i = 0;
					while (i < window->cursor_x) {
						new_line[end] = window->text->text[window->cursor_y][i];
						end++;
						i++;
					}
					i++;
					while (i < line_len) {
						new_line[end] = window->text->text[window->cursor_y][i];
						end++;
						i++;
					}
					new_line[end] = (char_t){.ch = L'\0', .fg = 0, .bg = 0, .bold = 0, .underline = 0, .reverse = 0, .blink = 0};
					free(window->text->text[window->cursor_y]);
					window->text->text[window->cursor_y] = new_line;
				}
			}
			return 1;
		}
		if (key == -1 || key == 0x1b)
			return 2;
		return 0;
	}
	if (isprint(key)) {
		int old_cursor_x = window->cursor_x;
		int line_len = 0;
		while (window->text->text[window->cursor_y][line_len].ch != '\0')
			line_len++;
		if (window->cursor_x >= line_len) {
			window->cursor_x = line_len - 1;
		}

		line_len = 0;
		while (window->text->text[window->cursor_y][line_len].ch != '\0')
			line_len++;
		char_t *new_line = malloc(sizeof(char_t) * (line_len + 2));
		if (new_line == NULL)
			return 0;
		for (int i = 0; i < window->cursor_x; i++)
			new_line[i] = window->text->text[window->cursor_y][i];
		new_line[window->cursor_x] = (char_t){.ch = key, .fg = 0, .bg = 0, .bold = 0, .underline = 0, .reverse = 0, .blink = 0};
		for (int i = window->cursor_x; i < line_len; i++)
			new_line[i + 1] = window->text->text[window->cursor_y][i];
		new_line[line_len + 1] = (char_t){.ch = '\0', .fg = 0, .bg = 0, .bold = 0, .underline = 0, .reverse = 0, .blink = 0};
		free(window->text->text[window->cursor_y]);
		window->text->text[window->cursor_y] = new_line;
		window->cursor_x = old_cursor_x + 1;
		if (window->cursor_x >= window->view_x + window->width) {
			window->view_x++;
		}
		return 1;
	}
	return 0;
}