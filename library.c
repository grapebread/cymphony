#include "library.h"

struct library *make_library()
{
    struct library *library = calloc(1, sizeof(struct library));
    library->album = NULL;
    library->next = NULL;
    return library;
}

struct library *add_to_library(struct library *data, char *filename)
{
    struct tags *new_mp3 = make_tag();
    new_mp3 = read_file_info(new_mp3, filename);
    struct library *head = data;
    if (data->album == NULL)
    {
        struct album *new_album = make_album();
        strcpy(new_album->name, new_mp3->album);
        new_album->data = new_mp3;
        data->album = new_album;
    }
    else
    {
        while (data != NULL)
        {
            if (!strcasecmp(data->album->name, new_mp3->album))
            {
                add_to_album(data->album, new_mp3);
                return head;
            }
            data = data->next;
        }
        struct album *new_album = make_album();
        strcpy(new_album->name, new_mp3->album);
        new_album->data = new_mp3;
        data = make_library();
        data->album = new_album;
        struct library *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = data;
    }
    return head;
}

struct library *remove_from_library(struct library *data, char *name)
{
    struct library *head = data;
    while (data)
    {
        struct album *tempalb = data->album;
        while (data->album)
        {
            if (!strcasecmp(data->album->data->title, name))
            {
                data->album = data->album->next;
                if (data->album == NULL)
                {
                    data->album = data->next->album;
                }
                return head;
            }
            data->album = data->album->next;
        }
        data->album = tempalb;
        data = data->next;
    }
    return head;
}

struct library *save_library(struct library *data)
{
    struct library *copy = make_library();
    int i = open("library.data", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    while (data)
    {
        while (data->album)
        {
            copy = add_to_library(copy, data->album->data->path);
            write(i, data->album->data->path, sizeof(data->album->data->path));
            data->album = data->album->next;
        }
        data = data->next;
    }
    return copy;
}

struct library *read_library()
{
    int i = open("library.data", O_RDONLY);
    struct library *new_library = make_library();
    char path[100];
    while (read(i, path, sizeof(path)))
    {
        new_library = add_to_library(new_library, path);
    }
    return new_library;
}

int get_library_len(struct library *data)
{
    int i = 0;
    while (data)
    {
        ++i;
        data = data->next;
    }

    return i;
}

struct library *get_nth_album(struct library *data, int n)
{
    int i = 0;
    while (data)
    {
        if (i == n)
        {
            return data;
        }

        ++i;
        data = data->next;
    }
}

// direction = 0 = ascending
// direction = 0 = descending
struct library *sort(struct library *data, int direction)
{
}