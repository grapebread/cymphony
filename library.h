#include "album.h"

struct library
{
  struct album *album;
  struct library *next;
};

struct library *make_library();
struct library *add_to_library(struct library *data, char *filename);
void save_library(struct library *data);
struct library *read_library();
void print_library(struct library *data);
int get_library_len(struct library *data);
struct library *get_nth_album(struct library *data, int n);
