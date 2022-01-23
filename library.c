#include "library.h"

struct library *make_library(){
  struct library *library = calloc(1, sizeof(struct library));
  library -> album = NULL;
  library -> next = NULL;
  return library;
}

struct library *add_to_library(struct library *data, char *filename){
  struct tags *new_mp3 = make_tag();
  new_mp3 = read_file_info(new_mp3, filename);
  struct library *head = data;
  if(data -> album == NULL){
    struct album *new_album = make_album();
    strcpy(new_album -> name, new_mp3 -> album);
    new_album -> data = new_mp3;
    data -> album = new_album;
  }
  else{
    while(data != NULL){
      if (!strcasecmp(data -> album -> name, new_mp3 -> album)){
        add_to_album(data -> album, new_mp3);
        return head;
      }
      data = data -> next;
    }
    struct album *new_album = make_album();
    strcpy(new_album -> name, new_mp3 -> album);
    new_album -> data = new_mp3;
    data = make_library();
    data -> album = new_album;
    struct library *temp = head;
    while(temp -> next != NULL){
      temp = temp -> next;
    }
    temp -> next = data;
  }
  return head;
}

struct library *remove_from_library(struct library *data, char *name){
  struct library *head = data;
  while (data){
    struct album *tempalb = data -> album;
    while (data -> album){
      if (!strcasecmp(data -> album -> data -> title, name)){
        data -> album = data -> album -> next;
        if (data -> album == NULL){
          data -> album = data -> next -> album;
        }
        return head;
      }
      data -> album = data -> album -> next;
    }
    data -> album = tempalb;
    data = data -> next;
  }
  return head;
}
