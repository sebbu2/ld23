DEBUG=-O0 -g -D_DEBUG
CPPFLAGS=$(DEBUG) -W -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors -Wshadow -Wconversion
# -Winvalid-pch -fpch-preprocess
CFLAGS=-std=c99 -Wc++-compat
CXXFLAGS=-Wsign-conversion -Woverloaded-virtual -std=gnu++98 -Wno-long-long -fno-rtti -Wc++0x-compat
#LDFLAGS=-Wl,--as-needed -WL,--enable-auto-import

INCLUDES=
LDFLAGS=
#SDL_mixer
INCLUDES+=-I/q/SDL/SDL_mixer-1.2.12/include
LDFLAGS+=-L/q/SDL/SDL_mixer-1.2.12/lib/x86 -lSDL_mixer
#SDL_ttf
INCLUDES+=-I/q/SDL/SDL_ttf-2.0.11/include
LDFLAGS+=-L/q/SDL/SDL_ttf-2.0.11/lib/x86 -lSDL_ttf
#SDL_gfx
INCLUDES+=-I/q/SDL/SDL_gfx/include
LDFLAGS+=-L/q/SDL/SDL_gfx/lib -lSDL_gfx
#SDL_image
INCLUDES+=-I/q/SDL/SDL_image-1.2.12/include
LDFLAGS+=-L/q/SDL/SDL_image-1.2.12/lib/x86 -lSDL_image
#SDL
INCLUDES+=-I/q/SDL/SDL-1.2.15/include/SDL -D_GNU_SOURCE=1 -Dmain=SDL_main
LDFLAGS+=-L/q/SDL/SDL-1.2.15/lib -lmingw32 -lSDLmain -lSDL -mwindows

#disable built-in implicit rules
.SUFFIXES:
#disable automatic deletion of intermediate files
.SECONDARY:
#delete output file if error occured
.DELETE_ON_ERROR:

all: main.exe
main.exe: main.cc.o level.cc.o position.cc.o collision.cc.o global.cc.o
main.cc.o: level.hpp position.hpp collision.hpp global.hpp
level.cc.o: global.hpp
position.cc.o: global.hpp
collision.cc.o: position.hpp global.hpp

%.exe: %.cc.o
	$(CXX) -o $@ $(filter %.o,$^) $(LDFLAGS) $(LOADLIBES) $(LDLIBS)

%.cc.o: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -o $@ -c $(filter %.cpp,$^)

clean:
	@-rm *.o 2>/dev/null
	@-rm *.exe 2>/dev/null
