#include "gui.h"

WINDOW **setup(WINDOW **ctrl_win)
{
    initscr();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);

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

    mvwprintw(album_title, 2, (x_max - strlen("Albums")) / 2, "Albums");
    mvwprintw(track_title, 2, (x_max - strlen("Tracks")) / 2, "Tracks");
    mvwprintw(info_title, 2, (x_max - strlen("Info & Queue")) / 2, "Info & Queue");

    refresh();
    wrefresh(album_title);
    wrefresh(track_title);
    wrefresh(info_title);

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

void create_input_window(char *message, char *input)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    WINDOW *win = create_win(6, 80, (y_max / 2) - 4, (x_max / 2) - 40);
    PANEL *pan = new_panel(win);

    mvwprintw(win, 2, 2, message);
    wmove(win, 3, 2);
    wrefresh(win);
    nodelay(stdscr, false);
    curs_set(1);
    echo();
    wgetstr(win, input);
    noecho();
    curs_set(0);
    nodelay(win, true);
    del_panel(pan);
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

int create_selection_window(char **selections, char *message, int size)
{
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    WINDOW *win = create_win(size + 8, 50, (y_max / 2) - 10, (x_max / 2) - 25);
    PANEL *pan = new_panel(win);
    mvwprintw(pan->win, 2, 2, "Sort by %s?", message);
    wrefresh(win);

    int highlight = 0;

    while (true)
    {
        for (int i = 0; i < size; ++i)
        {
            if (i == highlight)
                wattron(pan->win, A_REVERSE);
            mvwprintw(pan->win, i + 4, 2, selections[i]);
            wattroff(pan->win, A_REVERSE);
        }

        int c = wgetch(pan->win);

        switch (c)
        {
        case 65:
            if (!(highlight <= 0))
                --highlight;
            break;
        case 66:
            if (!(highlight >= size - 1))
                ++highlight;
            break;
        case 10:
            return highlight;
            break;
        default:
            break;
        }
    }
}