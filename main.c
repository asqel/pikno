
#include "file.h"
#include "win.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>

#define FPS 60
// é

int main(int argc, char **argv) {
	wchar_t *file = read_file("main.c");
	if (file == NULL) {
		printf("Error reading file\n");
		return 1;
	}
	window_t win = new_window(1, 1, 80, 25, file);
	display_init();
	FILE *f = fopen("log.txt", "w");
	int nee_refresh = 1;
	while (1) {

		if (nee_refresh)
		{
			display_wins(&win, 1);
			display_update();
			nee_refresh = 0;
		}
		int c = getch();
		if (c == 'q') {
			break;
		}
		if (c == 'w') {
			win.cursor_y--;
			nee_refresh = 1;
		}
		if (c == 's') {
			win.cursor_y++;
			nee_refresh = 1;
		}
		if (c == 'a') {
			win.cursor_x--;
			nee_refresh = 1;
		}
		if (c == 'd') {
			win.cursor_x++;
			nee_refresh = 1;
		}
		usleep(1000 * 1000 / FPS);
		fprintf(f, "%d %d\n", win.cursor_x, win.cursor_y);
		fflush(f);

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

