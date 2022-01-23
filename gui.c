#include <ncurses.h>
#include <menu.h>
#include <string.h>

char *albums[] =
    {
        "Assortment of sense",
        "Cyrstal Stone",
        "TOHO BOSSA NOVA",
        "RETRO FUTURE GIRLS",
        "disco metric",
        "Adrastea",
        "Sedecim",
        "Solar",
        "Broad Border",
        "emotional feedback",
        "Dream Materialise"};

char *aos_tracks[] =
    {
        "Sukima Paradise",
        "Illusionary Girl",
        "Welcome to Owen's House",
        "fof",
        "The Gensokyo the Misfortune Gods Loved",
        "night falls",
        "Shooting Star Sparkling in the Sky ~ Meteor of magic",
        "Dream of utopia",
        "Lost Illusions",
        "Now, quietly, the millennium..",
        "Wind God Girl ~ eXceed mix",
        "Unconfirmed frog fights syndrome"};

char *rfg_tracks[] =
    {
        "Super Fine Red",
        "Ready Made City Life",
        "Cherry Blossom Crash",
        "MyonMyonMyonMyonMyonMyonMyon!",
        "Fly Out! Bankikki",
        "Spring Rouge",
        "Desert Years",
        "Phoenix"};

int main(void)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    init_pair(1, COLOR_RED, COLOR_BLACK);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int title_pad = 3;
    int height = yMax - 8 - title_pad;
    int width = (xMax / 3);

    // Albums
    WINDOW *albumwin = newwin(height, width, title_pad, 1);
    box(albumwin, 0, 0);
    refresh();
    wrefresh(albumwin);

    WINDOW *albumtitle = newwin(title_pad, width, 0, 1);
    box(albumtitle, 0, 0);
    refresh();
    wrefresh(albumtitle);

    // Tracks
    WINDOW *trackwin = newwin(height, width, title_pad, width + 1);
    box(trackwin, 0, 0);
    refresh();
    wrefresh(trackwin);

    WINDOW *tracktitle = newwin(title_pad, width, 0, width + 1);
    box(tracktitle, 0, 0);
    refresh();
    wrefresh(tracktitle);

    // Track Info
    WINDOW *infowin = newwin(height, width, title_pad, width * 2 + 1);
    box(infowin, 0, 0);
    refresh();
    wrefresh(infowin);

    WINDOW *infotitle = newwin(title_pad, width, 0, width * 2 + 1);
    box(infotitle, 0, 0);
    refresh();
    wrefresh(infotitle);

    // Bar
    WINDOW *barwin = newwin(8, xMax - 2, yMax - 8, 1);
    box(barwin, 0, 0);
    refresh();
    wrefresh(barwin);

    keypad(albumwin, true);
    keypad(trackwin, true);
    WINDOW *screens[] = {albumwin, trackwin};

    int choice;
    int highlight = 0;

    while (TRUE)
    {
        for (int i = 0; i < 11; ++i)
        {
            if (i == highlight)
                wattron(albumwin, A_REVERSE);
            mvwprintw(albumwin, i + 2, 2, albums[i]);
            wattroff(albumwin, A_REVERSE);
        }

        if (!strcmp(albums[highlight], "Assortment of sense"))
        {
            for (int i = 0; i < 12; ++i)
                mvwprintw(trackwin, i + 2, 2, aos_tracks[i]);
        }
        else if (!strcmp(albums[highlight], "RETRO FUTURE GIRLS"))
        {
            for (int i = 0; i < 8; ++i)
                mvwprintw(trackwin, i + 2, 2, rfg_tracks[i]);
        }
        else
        {
            werase(trackwin);
            box(trackwin, 0, 0);
        }

        wrefresh(trackwin);

        choice = wgetch(albumwin);

        switch (choice)
        {
        case KEY_UP:
            if (!(highlight <= 0))
                --highlight;
            break;
        case KEY_DOWN:
            if (!(highlight >= 10))
                ++highlight;
            break;
        default:
            break;
        }
    }

    getch();
    endwin();
}
