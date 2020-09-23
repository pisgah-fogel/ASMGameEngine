SOURCES = main.c
SDL_LINKER_OPT = -lSDL2main -lSDL2 -lSDL2_image
CHIPMUNK_LINKER_OPT = -lchipmunk
ifeq ($(OS),Windows_NT)
	SDL_INCLUDE = "C:/SDL2/i686-w64-mingw32/include"
	SDL_LIB = -L"C:/SDL2/i686-w64-mingw32/lib"
	GCC_LIB = -L"C:/MinGW/lib"
    CHIPMUNK_LIB = -L"C:/Chipmunk/src"
    CHIPMUNK_INCLUDE = "C:/Chipmunk/include"
    SYSTEM_OPT = -w -Wl,-subsystem,windows -lmingw32
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
    CHIPMUNK_INCLUDE = /usr/include/chipmunk
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
	gcc ${SOURCES} -I${SDL_INCLUDE} -I${CHIPMUNK_INCLUDE} -g ${SDL_LIB} ${GCC_LIB} ${CHIPMUNK_LIB} ${SYSTEM_OPT} ${SDL_LINKER_OPT} ${CHIPMUNK_LINKER_OPT} -o debug

clean:
	rm -f debug
