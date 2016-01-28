UNAME := $(shell uname)

COMPILER = gcc

FLAGS= -I.  --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra -L. 
LDFLAGS= -lm

SOURCES=$(wildcard queues/*.c)
HEADERS=$(wildcard queues/*.h)

all: clean dmxMidiCtrlNg

clean:
	@rm -f dmxMidiCtrlNg


dmxMidiCtrlNg: $(SOURCES) $(HEADERS) main.c main.h dmx_image.c dmx_image.h dmx_devices.c dmx_devices.h dmx_channels.c dmx_channels.h dmx_queue.c dmx_queue.h Makefile 
	@echo "  \033[1;34mCompile\033[0m"
	@$(COMPILER) main.c dmx_queue.c dmx_image.c dmx_devices.c dmx_channels.c $(SOURCES) -o dmxMidiCtrlNg -I. $(FLAGS) $(LDFLAGS) 

.PHONY : clean all 

