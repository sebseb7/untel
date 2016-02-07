UNAME := $(shell uname)

COMPILER = gcc

FLAGS= -O0 -g -I. -Ilibs --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra -L.
LDFLAGS= -lm -lportmidi -lftdi1 -llo

SOURCES=$(wildcard queues/*.c libs/*.c)
HEADERS=$(wildcard queues/*.h libs/*.h)

all: dmxMidiCtrlNg

clean:
	@rm -f dmxMidiCtrlNg
	@rm -f git.log


dmxMidiCtrlNg: $(SOURCES) $(HEADERS) main.c main.h dmx_output.c dmx_output.h dmx_selector.c dmx_selector.h dmx_image.c dmx_image.h dmx_devices.c dmx_devices.h dmx_channels.c dmx_channels.h dmx_queue.c dmx_queue.h Makefile 
	@rm -f dmxMidiCtrlNg
	@echo "  \033[1;34mCompile\033[0m"
	@$(COMPILER) main.c dmx_output.c dmx_selector.c dmx_queue.c dmx_image.c dmx_devices.c dmx_channels.c $(SOURCES) -o dmxMidiCtrlNg $(FLAGS) $(LDFLAGS) 

gource:
	git log --reverse --pretty=format:"%at|%B" > git.log
	gource --caption-file git.log --caption-duration 3 --caption-size 10 -max-files 99999 -disable-progress -stop-at-end  -user-scale 1 -highlight-all-users .
	rm git.log

.PHONY : clean all gource

