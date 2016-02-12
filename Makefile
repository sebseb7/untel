UNAME := $(shell uname)

COMPILER = gcc

FLAGS= -O0 -g -I. -Ilibs -Idmx -I/usr/include/lua5.2 --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra -L.
LDFLAGS= -lm -lportmidi -lftdi1 -llo -llua5.2

SOURCES=$(wildcard queues/*.c libs/*.c dmx/*.c)
HEADERS=$(wildcard queues/*.h libs/*.h dmx/*.h)

FLAGS+=`sdl2-config --cflags`
LDFLAGS+=`sdl2-config --libs`

all: dmxMidiCtrlNg

plot:
	./dmxMidiCtrlNg | ./feedgnuplot --stream trigger --lines --domain --xlen 10000 --ymin 0 --ymax 256 --terminal 'x11'

clean:
	@rm -f dmxMidiCtrlNg
	@rm -f git.log


dmxMidiCtrlNg: $(SOURCES) $(HEADERS) main.c main.h Makefile 
	@clear
	@rm -f dmxMidiCtrlNg
	@echo "  \033[1;34mCompile\033[0m"
	@$(COMPILER) main.c $(SOURCES) -o dmxMidiCtrlNg $(FLAGS) $(LDFLAGS) 

gource:
	git log --reverse --pretty=format:"%at|%B" > git.log
	gource --caption-file git.log -a 1 --caption-duration 3 --caption-size 10 -max-files 99999 -disable-progress -stop-at-end  -user-scale 1 -highlight-all-users .
	rm git.log

.PHONY : clean all gource plot

