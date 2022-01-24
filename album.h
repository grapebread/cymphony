#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "id3reader.h"

struct album
{
    char name[252];
    struct tags *data;
    struct album *next;
};

struct album *make_album();
struct album *add_to_album(struct album *data, struct tags *new_mp3);
void save_album(struct album *data, struct tags *new_mp3);
struct album *get_nth_track(struct album *data, int n);
int get_album_len(struct album *data);
struct album *sorted_insert_duration_des(struct album *head, struct album *node);
struct album *sorted_insert_duration_asc(struct album *head, struct album *node);
struct album *sorted_insert_artist_asc(struct album *head, struct album *node);
struct album *sorted_insert_artist_des(struct album *head, struct album *node);
struct album *sorted_insert_name_asc(struct album *head, struct album *node);
struct album *sorted_insert_name_des(struct album *head, struct album *node);
