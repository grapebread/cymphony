#include <stdio.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define TRUE 1
#define FALSE 0

// Some hack to stop "undefined reference to 'WinMain'" errors
#ifdef main
#undef main
#endif

int main(int argc, char *args[])
{
    if (argc != 2)
    {
        printf("One argument must be given (path to music file).\n");
        return -1;
    }
    char *path = args[1];

    Mix_Music *music = NULL;

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

    // Load audio file
    music = Mix_LoadMUS(path);
    if (music == NULL)
    {
        printf("Failed to load music. Error: %s\n", Mix_GetError());
        return -1;
    }

    // Play track and set volume to half
    int i = Mix_PlayMusic(music, 1);
    Mix_VolumeMusic(10);

    int running = TRUE;
    while (running)
    {
        printf("working\n");
        if (!Mix_PlayingMusic())
        {
            return 1;
        }
    }

    // Release resources
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}