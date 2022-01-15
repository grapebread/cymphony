#include "reader.h"

int main(int argc, char *argv[]){
  if (!argc){
    printf("Please select mp3 file");
  }
  else{
    struct tags mp3;
    FILE *file;
    printf("%s\n", argv[1]);
    file = fopen(argv[1], "rb");
    if (file){
      fseek(file, -1 * sizeof(struct tags), SEEK_END);
      fread(&mp3, sizeof(struct tags), 1, file);
      printf("Title: %s\n", mp3.title);
      printf("Artist: %s\n", mp3.artist);
      printf("Album: %s\n", mp3.album);
      printf("Year: %s\n", mp3.year);
      printf("Comment: %s\n", mp3.comment);
      printf("Genre: %uc\n", mp3.genre);
    }
    else{
      printf("Couldn't open file\n");
    }
  }
}
