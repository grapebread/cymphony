all: main.o
	gcc -Isrc/include -Lsrc/lib -o copium main.o -lSDL2main -lSDL2 -lSDL2_mixer

main.o:	main.c
	gcc -Isrc/include -Lsrc/lib -c main.c -lSDL2main -lSDL2 -lSDL2_mixer

run:
	./copium

clean:
	@rm *.o *.exe *.gch || true
