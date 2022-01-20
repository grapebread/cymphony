all: main.o
	gcc -o copium main.o `sdl2-config --cflags --libs` -lSDL2_mixer

main.o:	main.c
	gcc -c main.c `sdl2-config --cflags --libs` -lSDL2_mixer

gui:
	gcc -o gui gui.c -lncurses -lmenu

run:
	./copium

clean:
	@rm *.o *.exe *.gch || true
