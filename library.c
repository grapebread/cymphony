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
    close(i);
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
    close(i);
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

struct library *sort(struct library *data, int direction)
{
    if (direction == 0)
    {
        struct library *current = data;
        struct library *sorted = NULL;
        while (current != NULL)
        {
            struct library *next = current->next;
            sorted = sortedInsertAsc(sorted, current);
            current = next;
        }
        return sorted;
    }
    else
    {
        struct library *current = data;
        struct library *sorted = NULL;
        while (current != NULL)
        {
            struct library *next = current->next;
            sorted = sortedInsertDes(sorted, current);
            current = next;
        }
        return sorted;
    }
}

struct library *sortedInsertAsc(struct library *head, struct library *node)
{
    if (head == NULL || strcasecmp(head->album->name, node->album->name) > 0)
    {
        node->next = head;
        return node;
    }
    else
    {
        struct library *current = head;
        while (current->next != NULL && strcasecmp(current->next->album->name, node->album->name) < 0)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    return head;
}

struct library *sortedInsertDes(struct library *head, struct library *node)
{
    if (head == NULL || strcasecmp(head->album->name, node->album->name) < 0)
    {
        node->next = head;
        return node;
    }
    else
    {
        struct library *current = head;
        while (current->next != NULL && strcasecmp(current->next->album->name, node->album->name) > 0)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    return head;
}

struct library *sort_album(struct library *data, int direction, int type)
{
    struct album *current = data->album;
    struct album *sorted = NULL;

    while (current != NULL)
    {
        struct album *next = current->next;

        if (direction == 0)
        {

            if (type == 0)
                sorted = sorted_insert_name_asc(sorted, current);
            if (type == 1)
                sorted = sorted_insert_artist_asc(sorted, current);
            if (type == 2)
                sorted = sorted_insert_duration_asc(sorted, current);
        }
        else
        {
            if (type == 0)
                sorted = sorted_insert_name_des(sorted, current);
            if (type == 1)
                sorted = sorted_insert_artist_des(sorted, current);
            if (type == 2)
                sorted = sorted_insert_duration_des(sorted, current);
        }

        current = next;
    }

    data->album = sorted;
    return data;
}
