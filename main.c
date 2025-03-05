
#include "file.h"
#include "win.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>

#define FPS 30
// é

int main(int argc, char **argv) {
	wchar_t *file = read_file("main.c");
	if (file == NULL) {
		printf("Error reading file\n");
		return 1;
	}

	window_t win = new_window(1, 1, 80, 25, file);
	display_init();
	//printf("truc %d\n", win.text->text[9][3].ch);
	//mvaddch(0, 0, win.text->text[9][3].ch);
	//refresh();
//
	//return 0;

	while (1) {
		display_wins(&win, 1);
		display_update();
		usleep(1000 * 1000 / FPS);

		int c = getch();
		if (c == 'q') {
			break;
		}
		if (c == 'w') {
			win.view_y--;
		}
		if (c == 's') {
			win.view_y++;
		}
		if (c == 'a') {
			win.view_x--;
		}
		if (c == 'd') {
			win.view_x++;
		}
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

