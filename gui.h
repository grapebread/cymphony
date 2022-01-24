#ifndef GUI_H
#define GUI_H

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <panel.h>

#define NUM_WIN 4

WINDOW **setup(WINDOW **ctrl_win);
WINDOW *create_win(int height, int width, int start_y, int start_x);
void create_status_window(char *message);
int create_selection_window(char **selections, int size);
void create_input_window(char *message, char *input);

#endif