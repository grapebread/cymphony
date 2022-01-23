all: main.o album.o id3reader.o library.o
	gcc -o copium main.o album.o id3reader.o library.o `sdl2-config --cflags --libs` -lSDL2_mixer -lavformat -lavutil -lavcodec -lncurses -lmenu

main.o:	main.c album.h id3reader.h library.h
	gcc -c main.c `sdl2-config --cflags --libs` -lSDL2_mixer

album.o: album.c album.h id3reader.h
		gcc -c album.c

id3reader.o: id3reader.c id3reader.h
		gcc -c id3reader.c

library.o: library.c library.h album.h id3reader.h
		gcc -c library.c

gui:
	gcc -o gui gui.c -lncurses -lmenu

run:
	./copium

clean:
	@rm *.o *.exe *.gch || true
