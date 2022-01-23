#include "gui.h"

WINDOW **setup(WINDOW **ctrl_win)
{
    initscr();
    nodelay(stdscr, true);
    noecho();
    // curs_set(0);

    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    int title_pad = 3;
    int height = y_max - 8 - title_pad;
    int width = (x_max / 3);

    // 0 - Albums
    // 1 - Tracks
    // 2 - Track info
    // 3 - Bottom bar
    ctrl_win[0] = create_win(height, width, title_pad, 1);
    ctrl_win[1] = create_win(height, width, title_pad, width + 1);
    ctrl_win[2] = create_win(height, width, title_pad, width * 2 + 1);
    ctrl_win[3] = create_win(8, x_max - 2, y_max - 8, 1);

    WINDOW *album_title = create_win(title_pad, width, 0, 1);
    WINDOW *track_title = create_win(title_pad, width, 0, width + 1);
    WINDOW *info_title = create_win(title_pad, width, 0, width * 2 + 1);

    keypad(ctrl_win[0], true);
    keypad(ctrl_win[1], true);

    return ctrl_win;
}

WINDOW *create_win(int height, int width, int start_y, int start_x)
{
    WINDOW *win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    refresh();
    wrefresh(win);

    return win;
}

void create_status_window(char *message)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    WINDOW *win = create_win(4, 40, (y_max / 2) - 4, (x_max / 2) - 40);
    PANEL *pan = new_panel(win);
    mvwprintw(pan->win, 1, 1, message);
    mvwprintw(pan->win, 2, 1, "Press any key to continue.");
    wrefresh(win);
    while (true)
    {
        if (wgetch(pan->win) != -1)
        {
            break;
        }
    }
    del_panel(pan);
}