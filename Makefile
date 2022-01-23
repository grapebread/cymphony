all: main.o library.o album.o id3reader.o gui.o
	gcc -o copium main.o gui.o library.o album.o id3reader.o `sdl2-config --cflags --libs` -lavformat -lavutil -lavcodec -lncurses -lpanel -lSDL2_mixer

main.o: main.c gui.c gui.h library.c library.h album.c album.h id3reader.c id3reader.h
	gcc -c main.c

library.o: library.c library.h album.o album.h id3reader.c id3reader.h
	gcc -c library.c

album.o: album.c album.h id3reader.c id3reader.h
	gcc -c album.c

id3reader.o: id3reader.c id3reader.h
	gcc -c id3reader.c

gui.o: gui.c gui.h
	gcc -c gui.c

run:
	./copium

clean:
	@rm *.o *.exe *.gch || true
