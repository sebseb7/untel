UNAME := $(shell uname)

COMPILER = gcc

ifeq ($(UNAME), Darwin)
	COMPILER = clang
endif

DMX_OUT=0
SDL_OUT=1
LUA_CUE=0



SOURCES=$(wildcard main.c queues/*.c queues/par56/*.c queues/strobe/*.c libs/*.c libs/mcugui/*.c libs/gui/*.c dmx/*.c)

ifeq (0,${DMX_OUT})
SOURCES := $(filter-out dmx/dmx_output.c, $(SOURCES))
endif

ifeq (0,${SDL_OUT})
SOURCES := $(filter-out $(wildcard libs/menu.c libs/sdl_util.c libs/gui/*.c libs/mcugui/*.c), $(SOURCES))
endif

DEPS   =$(patsubst %,.bin/%,$(SOURCES:.c=.d)) 
OBJECTS=$(patsubst %,.bin/%,$(SOURCES:.c=.o)) 

FLAGS= -O0 -g -I. -Ilibs -Idmx --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra 
LDFLAGS= -lm -lportmidi -llo 

ifeq (1,${LUA_CUE})
LUA_VERSION=$(shell pkg-config --list-all | grep lua | grep 5 | grep -v c | sort | cut -d \  -f 1 | tail -1)
FLAGS+=$(shell pkg-config --cflags ${LUA_VERSION})
LDFLAGS+=$(shell pkg-config --libs ${LUA_VERSION})
else
SOURCES := $(filter-out $(wildcard dmx/dmx_luaqueue.c queues/lua_queues.c), $(SOURCES))
LUA_VERSION="no lua"
endif


ifeq (1,${DMX_OUT})
LDFLAGS+= -lftdi1
endif

ifeq (1,${SDL_OUT})
FLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs)
endif

FLAGS+= -DDMX_OUT=$(DMX_OUT) -DSDL_OUT=$(SDL_OUT) -DLUA_CUE=$(LUA_CUE)

all: dmxMidiCtrlNg

plot:
	./dmxMidiCtrlNg | ./feedgnuplot --stream trigger --lines --domain --xlen 10000 --ymin 0 --ymax 256 --terminal 'x11'

clean:
	@rm -f $(DEPS)
	@rm -f $(OBJECTS)
	@rm -fr .bin
	@rm -f dmxMidiCtrlNg
	@rm -f git.log

dmxMidiCtrlNg: $(OBJECTS) Makefile 
	@rm -f dmxMidiCtrlNg
	@echo "  \033[1;36mLink   \033[0m dmxMidiCtrlNg (using ${LUA_VERSION})"
	@$(COMPILER) $(OBJECTS) $(LDFLAGS)  -o dmxMidiCtrlNg

-include $(DEPS)

.bin/%.o: %.c Makefile
	@echo "  \033[1;34mCompile\033[0m $<"
	@mkdir -p $(dir $@)
	@$(COMPILER) $(FLAGS) -o $@ -c $<
	@$(COMPILER) $(FLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

gource:
	git log --reverse --pretty=format:"%at|%B" > git.log
	gource --seconds-per-day 1 --caption-file git.log -a 1 --caption-duration 7 --caption-size 13 -max-files 99999 -disable-progress -stop-at-end  -user-scale 1 -highlight-all-users -1280x720 -o - . | ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i - -vcodec libx264 -preset ultrafast -pix_fmt yuv420p -crf 1 -threads 0 -bf 0 gource.mp4
	rm git.log

kext: 
	sudo kextunload -b com.apple.driver.AppleUSBFTDI
	sudo kextunload -b com.FTDI.driver.FTDIUSBSerialDriver

.PHONY : clean all gource plot kext

