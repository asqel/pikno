[2J#include "file.h"                                                             � 
#include "win.h"                      � #include <unistd.h>                     
#include <stdio.h>                                                              
#include <unistd.h>                           � int main(int argc, char **argv) 
#define CLEAR "\x1b[2J"                                                         
int main(int argc, char **argv) {                                     a 	if (fil
	char *file = read_file("main.c");                                              
	if (file == N� 		printf("Error reading file\n");             ! 	}              
LL) { � �G� �G5 �G� �G� �G  �GG �G� �G� �G�  G@ �G� �G  �G0 �G� �G� �G� �Gp �G  
		printf("Error reading file\n");             ! 	}                              
		return 1;                                                                     
	}                                                    Q 	free(file);            
	window_t win = new_window(0, 0, 80, 25, file);               Q 	while (1) {    
	free(file);                                                                    
	while (1) {                   		for (int i = 0; i < win.height; i++) {        
		printf(CLEAR);                                                                
		for (int i = 0; i < win.height; i++) {                                        
			for (int j = 0; j < win.width; j++) {                                      ! 
				printf("%c", win.text[i][j].ch);                                          A 
			}                  ! 		}                                           ! }       
			printf("\n");      Q 		sleep(3);           ! 	}                    �         
		}                                           ! }                               
		sleep(3);           ! 	}                    �                                 
		break;      Q 	return 0;                                                     
	}                    �                                                         
