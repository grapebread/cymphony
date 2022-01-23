#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "library.h"

#define TRUE 1
#define FALSE 0

// Some hack to stop "undefined reference to 'WinMain'" errors
#ifdef main
#undef main
#endif


int main(int argc, char *args[])
{
  if (argc < 2)
  {
      printf("One argument must be given (path to music file).\n");
      return -1;
  }
  struct library *temp = make_library();
  for (int i = 1; i < argc; i++){
    temp = add_to_library(temp, args[i]);
  }
  int fd[2];
  int fd2[2];
  pipe(fd);
  pipe(fd2);
  int f = fork();
  if (f){
      fcntl(fd2[1], F_SETFL, O_NONBLOCK);
      close(fd[0]);
      initscr();
      cbreak();
      noecho();
      nodelay(stdscr, TRUE);
      scrollok(stdscr, TRUE);
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
      mvwprintw(albumtitle, 1, 2, "Albums");
      refresh();
      wrefresh(albumtitle);

      // Tracks
      WINDOW *trackwin = newwin(height, width, title_pad, width + 1);
      box(trackwin, 0, 0);
      refresh();
      wrefresh(trackwin);

      WINDOW *tracktitle = newwin(title_pad, width, 0, width + 1);
      box(tracktitle, 0, 0);
      mvwprintw(tracktitle, 1, 2, "Tracks");
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
      int highlight_albums = 0;
      int highlight_tracks = 0; // determines whether or not it's looking at albums
      int which_album = 0;
      struct album *list = make_album();
      while (TRUE)
      {
        werase(trackwin);
        werase(barwin);
        werase(infowin);
        box(barwin, 0, 0);
        box(trackwin, 0, 0);
        box(infowin, 0, 0);
        int status;
        struct library *head = temp;
        int total_library_size = 0;
        while(temp)
        {
          temp = temp -> next;
          total_library_size++;
        }
        int i = 0;
        temp = head;
        while(temp)
        {
            if (i == highlight_albums && highlight_tracks == 0)
                wattron(albumwin, A_REVERSE);
            mvwprintw(albumwin, i + 2, 2, temp -> album -> name);
            wattroff(albumwin, A_REVERSE);
            temp = temp -> next;
            i++;
        }
        temp = head;
        i = 0;
        if (highlight_tracks == 1){
          for (int i = 0; i < which_album; i++){
            temp = temp -> next;
          }
        }
        else{
          for (int i = 0; i < highlight_albums; i++){
            temp = temp -> next;
          }
        }
        struct album *tempalb = temp -> album;
        int total_album_size = 0;
        while(temp -> album)
        {
          temp -> album = temp -> album -> next;
          total_album_size++;
        }
        temp -> album = tempalb;
        i = 0;
        while(temp -> album){
          if (i == highlight_albums && highlight_tracks == 1){
              wattron(trackwin, A_REVERSE);
              mvwprintw(infowin, 2, 2, temp -> album -> data -> title);
              mvwprintw(infowin, 3, 2, temp -> album -> data -> artist);
              mvwprintw(infowin, 4, 2, temp -> album -> data -> album);
              mvwprintw(infowin, 5, 2, temp -> album -> data -> date);
              mvwprintw(infowin, 6, 2, temp -> album -> data -> genre);
          }
          mvwprintw(trackwin, i + 2, 2, temp -> album -> data -> title);
          wattroff(trackwin, A_REVERSE);
          temp -> album = temp -> album -> next;
          i++;
        }
        temp -> album = tempalb;
        wrefresh(trackwin);
        wrefresh(infowin);

        choice = wgetch(albumwin);
        switch (choice)
        {
          case KEY_UP:
              if (!(highlight_albums <= 0))
                  --highlight_albums;
              break;
          case KEY_DOWN:
              if (highlight_tracks == 0)
                if (!(highlight_albums >= total_library_size))
                    ++highlight_albums;
                if (highlight_albums == total_library_size){
                  highlight_albums = 0;
                }
              else if (highlight_tracks == 1){
                if (!(highlight_albums >= total_album_size))
                  ++highlight_albums;
                if (highlight_albums == total_album_size){
                  highlight_albums = 0;
                }
              }
              break;
          case KEY_RIGHT:
              if (!(highlight_tracks >= 1)){
                  ++highlight_tracks;
                  which_album = highlight_albums;
                  highlight_albums = 0;
              }
              break;
          case KEY_LEFT:
            if (!(highlight_tracks <= 0)){
                --highlight_tracks;
                highlight_albums = which_album;
            }
            break;
          case 32:
            if (waitpid(f, &status, WNOHANG | WUNTRACED)){
              kill(f, SIGCONT);
            }
            else{
              kill(f, SIGSTOP);
            }
            break;
          case 62:
            write(fd2[1], "62", 10);
            if (list -> next != NULL)
              list = list -> next;
            break;
          case 10:
            if (highlight_tracks == 1){
              write(fd[1], temp -> album -> data -> path, sizeof(temp -> album -> data -> path));
              add_to_album(list, temp -> album -> data);
            }
          default:
            break;
          }
          temp = head;
      }

      getch();
      endwin();
    }
    else{
      fcntl(fd2[0], F_SETFL, O_NONBLOCK);
      if (SDL_Init(SDL_INIT_AUDIO) < 0)
      {
          printf("SDL could not initialise. Error:%s\n", SDL_GetError());
          return -1;
      }
    //
      if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
      {
          printf("SDL_mixer could not initialise. Error: %s\n", Mix_GetError());
          return -1;
      }

      while (1){
          char buffer[100] = {"/0"};
          read(fd[0], buffer, 100);
          Mix_Music *music = NULL;
          music = Mix_LoadMUS(buffer);
          if (music == NULL)
          {
              printf("Failed to load music. Error: %s\n", Mix_GetError());
              return -1;
          }
          // Play track and set volume to half
          int i = Mix_PlayMusic(music, 1);
          Mix_VolumeMusic(10);
          int running = TRUE;
          char status[10] = {"/0"};
          while(read(fd2[0], status, 10) != -1){};
          while (running)
          {
              char status[10] = {"/0"};
              read(fd2[0], status, 10);
              if (!Mix_PlayingMusic())
              {
                  break;
              }
              if (!(strcmp(status, "62"))){
                  Mix_HaltMusic();
              }

          }
      }
      // Release resources
      Mix_CloseAudio();
      SDL_Quit();
    }

    return 0;
}
