
#include "file.h"
#include "win.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

#define FPS 60
// é

void signal_handler(int sig) {
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}
	wchar_t *file = read_file(argv[1]);
	if (file == NULL) {
		printf("Error reading file\n");
		return 1;
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	window_t win = new_window(1, 1, 80, 25, file);
	//for (int i = 0; i < win.text->text_height; i++) {
	//	for (int k = 0; win.text->text[i][k].ch != '\0'; k++)
	//		printf("0x%x\n", win.text->text[i][k].ch);
	//}
//	return 0;
	win.is_focused = 1;
	display_init();
	win.cursor_y = 0;
	win.cursor_x = 0;
	int need_refresh = 1;
	while (1) {

		if (need_refresh)
		{
			int old_cursor_x = win.cursor_x;
			int line_len = 0;
			while (win.text->text[win.cursor_y][line_len].ch != '\0')
				line_len++;
			if (win.cursor_x >= line_len) {
				win.cursor_x = line_len - 1;
			}
			display_wins(&win, 1);
			display_update();
			need_refresh = 0;
			win.cursor_x = old_cursor_x;

		}
		int ret = handle_keys(&win, 1);
		if (ret == 1)
			need_refresh = 1;
		if (ret == 2)
			break;
		usleep(1000 * 1000 / FPS);
	}
	display_exit();
	//free(file);

	//initscr();
    //noecho();
    //curs_set(FALSE);
	//while (0) {
	//	clear();
	//	for (int i = 0; i < win.text->text_height; i++) {
	//		for (int j = 0; j < win.text->text[i][j].ch != '\0'; j++) {
	//			mvaddch(i, j, win.text->text[i][j].ch);
	//		}
	//	}
	//	refresh();
	//	sleep(3);
	//}

	return 0;
}

