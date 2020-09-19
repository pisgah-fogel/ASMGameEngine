all:
	gcc main.c -I/usr/include/SDL2 -lSDL2main -lSDL2 -lSDL2_image -o test1
	gcc physics.c -I/usr/include/SDL2 -I/usr/include/chipmunk -lSDL2main -lSDL2 -lSDL2_image -lchipmunk -o test2
clean:
	rm -f test1 test2 test3
windows:
	g++ main.c -IC:\SDL2\i686-w64-mingw32\include -LC:\SDL2\i686-w64-mingw32\lib -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o test1