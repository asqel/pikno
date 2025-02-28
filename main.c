
#include "file.h"
#include "win.h"
#include <stdio.h>
#include <unistd.h>

#define CLEAR "\x1b[2J"

int main(int argc, char **argv) {
	char *file = read_file("main.c");
	if (file == NULL) {
		printf("Error reading file\n");
		return 1;
	}

	window_t win = new_window(0, 0, 80, 25, file);
	free(file);


	while (1) {
		printf(CLEAR);
		for (int i = 0; i < win.text_height; i++) {
			for (int j = 0; j < win.text[i][j].ch != '\0'; j++) {
				printf("%c", win.text[i][j].ch);
			}
			printf("\n");
		}
		sleep(3);
		break;
	}

	return 0;
}

