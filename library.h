#include "album.h"

struct library{
  struct album *album;
  struct library *next;
};

struct library *make_library();
struct library *add_to_library(struct library *data, char *filename);
struct library *remove_from_library(struct library *data, char *name);
void save_library(struct library *data);
struct library *read_library();
