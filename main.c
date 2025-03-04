
#include "file.h"
#include "win.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *file = read_file("main.c");
	if (file == NULL) {
		printf("Error reading file\n");
		return 1;
	}

	window_t win = new_window(0, 0, 80, 25, file);
	for (int i = 0; i < win.text->text_height; i++) {
		if (win.text->text[i] == NULL) {
			continue;
		}
		for (int j = 0; j < win.text->text[i][j].ch != '\0'; j++) {
			printf("%c", win.text->text[i][j].ch);
		}
		printf("\n");
	}
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

