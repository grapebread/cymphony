#include <stdio.h>
#include "sys/wait.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "library.h"
#include "gui.h"

char *convert_sec_to_minsec(int time);

int main(void)
{
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    int f = fork();

    if (f)
    {
        fcntl(fd2[1], F_SETFL, O_NONBLOCK);
        close(fd1[0]);

        struct library *library = read_library();

        // 0 - Albums
        // 1 - Tracks
        // 2 - Track info
        // 3 - Bottom bar
        WINDOW *ctrl_win[NUM_WIN];
        setup(ctrl_win);

        struct album *queue = make_album();

        int choice;
        int highlight_album = 0;
        int highlight_track = 0;

        int i;
        while (TRUE)
        {
            int status;
            int library_len = get_library_len(library);

            i = 0;
            werase(ctrl_win[0]);
            box(ctrl_win[0], 0, 0);
            for (struct library *temp = library; temp; temp = temp->next, ++i)
            {
                if (i == highlight_album)
                    wattron(ctrl_win[0], A_REVERSE);
                mvwprintw(ctrl_win[0], i + 2, 2, temp->album->name);
                wattroff(ctrl_win[0], A_REVERSE);
            }

            struct library *curr_album = get_nth_album(library, highlight_album);

            i = 0;
            werase(ctrl_win[1]);
            box(ctrl_win[1], 0, 0);
            for (struct album *temp = curr_album->album; temp; temp = temp->next, ++i)
            {
                mvwprintw(ctrl_win[1], i + 2, 2, temp->data->title);
            }

            wrefresh(ctrl_win[0]);
            wrefresh(ctrl_win[1]);
            choice = wgetch(ctrl_win[0]);

            if (choice == 'q')
                break;

            switch (choice)
            {
            case KEY_UP:
                if (!(highlight_album <= 0))
                    --highlight_album;
                break;
            case KEY_DOWN:
                if (!(highlight_album >= library_len - 1))
                    ++highlight_album;
                break;
            case 'a':
                break;
            case 's':
                break;
            case 'w':
                break;
            case KEY_RIGHT:
                highlight_track = 0;
                while (TRUE)
                {
                    i = 0;
                    for (struct album *temp = curr_album->album; temp; temp = temp->next, ++i)
                    {
                        if (i == highlight_track)
                            wattron(ctrl_win[1], A_REVERSE);
                        mvwprintw(ctrl_win[1], i + 2, 2, temp->data->title);
                        wattroff(ctrl_win[1], A_REVERSE);
                    }

                    werase(ctrl_win[2]);
                    box(ctrl_win[2], 0, 0);
                    struct album *curr_track = get_nth_track(curr_album->album, highlight_track);
                    mvwprintw(ctrl_win[2], 2, 2, "Title: %s", curr_track->data->title);
                    mvwprintw(ctrl_win[2], 3, 2, "Artist: %s", curr_track->data->artist);
                    mvwprintw(ctrl_win[2], 4, 2, "Album: %s", curr_track->data->album);
                    mvwprintw(ctrl_win[2], 5, 2, "Genre: %s", curr_track->data->genre);
                    mvwprintw(ctrl_win[2], 6, 2, "Date Created: %s", curr_track->data->date);
                    mvwprintw(ctrl_win[2], 7, 2, "Duration: %s", convert_sec_to_minsec(curr_track->data->duration));

                    wrefresh(ctrl_win[1]);
                    wrefresh(ctrl_win[2]);
                    choice = wgetch(ctrl_win[1]);

                    if (choice == KEY_LEFT)
                    {
                        i = 0;
                        for (struct album *temp = curr_album->album; temp; temp = temp->next, ++i)
                        {
                            if (i == highlight_track)
                                mvwprintw(ctrl_win[1], i + 2, 2, temp->data->title);
                        }

                        break;
                    }

                    switch (choice)
                    {
                    case KEY_UP:
                        if (!(highlight_track <= 0))
                            --highlight_track;
                        break;
                    case KEY_DOWN:
                        if (!highlight_track >= 1)
                            ++highlight_track;
                        break;
                    case 32:
                        if (waitpid(f, &status, WNOHANG | WUNTRACED))
                            kill(f, SIGCONT);
                        else
                            kill(f, SIGSTOP);
                        break;
                    case 62:
                        write(fd2[1], "62", 10);
                        if (queue->next != NULL)
                            queue = queue->next;
                        break;
                    case 10:
                        write(fd1[1], curr_track->data->path, sizeof(curr_track->data->path));
                        add_to_album(queue, curr_track->data);
                        break;
                    default:
                        break;
                    }
                }
            default:
                break;
            }
        }

        endwin();
    }
    else
    {
        fcntl(fd2[0], F_SETFL, O_NONBLOCK);
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            printf("SDL could not initialise. Error:%s\n", SDL_GetError());
            return -1;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialise. Error: %s\n", Mix_GetError());
            return -1;
        }

        while (true)
        {
            char buffer[100] = {'\0'};
            read(fd1[0], buffer, sizeof(buffer));

            Mix_Music *music = NULL;
            music = Mix_LoadMUS(buffer);
            if (music == NULL)
            {
                printf("Failed to load music. Error: %s\n", Mix_GetError());
                return -1;
            }

            int i = Mix_PlayMusic(music, 1);
            Mix_VolumeMusic(10);

            int running = TRUE;
            char status[10] = {'\0'};
            while (read(fd2[0], status, sizeof(status)) != -1)
            {
            }

            while (running)
            {
                char status[10] = {'\0'};
                read(fd2[0], status, sizeof(status));

                if (!Mix_PlayingMusic())
                    break;
                if (!strcmp(status, "62"))
                    Mix_HaltMusic();
            }
        }

        Mix_CloseAudio();
        SDL_Quit();
    }

    return 0;
}

char *convert_sec_to_minsec(int time)
{
    int minutes = time / 60;
    int seconds = time % 60;

    char *formatted_time = calloc(6, sizeof(char));
    snprintf(formatted_time, 6, "%d:%d", minutes, seconds);

    return formatted_time;
}
