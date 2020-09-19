SDL_LINKER_OPT = -lSDL2main -lSDL2 -lSDL2_image
ifeq ($(OS),Windows_NT)
	SDL_INCLUDE = C:\SDL2\i686-w64-mingw32\include
	SDL_LIB = C:\SDL2\i686-w64-mingw32\lib
	MINGW_LIB = C:\MinGW\lib
	LIB_OPTION = -L${SDL_LIB} -L${MINGW_LIB}
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
else
	SDL_INCLUDE = /usr/include/SDL2
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

all:
	gcc main.c -I${SDL_INCLUDE} ${SDL_LINKER_OPT} -o test1
	gcc physics.c -I${SDL_INCLUDE} -I/usr/include/chipmunk ${SDL_LINKER_OPT} -lchipmunk -o test2
clean:
	rm -f test1 test2 test3
windows:
	g++ main.c -I${SDL_INCLUDE} ${LIB_OPTION} -w -Wl,-subsystem,windows -lmingw32 ${SDL_LINKER_OPT} -o test1