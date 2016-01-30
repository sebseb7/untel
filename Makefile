UNAME := $(shell uname)

COMPILER = gcc

FLAGS= -I. -Ilibs --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra -L.
LDFLAGS= -lm -lportmidi -lftdi1

SOURCES=$(wildcard queues/*.c libs/*.c)
HEADERS=$(wildcard queues/*.h libs/*.h)

all: dmxMidiCtrlNg

clean:
	@rm -f dmxMidiCtrlNg


dmxMidiCtrlNg: $(SOURCES) $(HEADERS) main.c main.h dmx_output.c dmx_output.h dmx_selector.c dmx_selector.h dmx_image.c dmx_image.h dmx_devices.c dmx_devices.h dmx_channels.c dmx_channels.h dmx_queue.c dmx_queue.h Makefile 
	@rm -f dmxMidiCtrlNg
	@echo "  \033[1;34mCompile\033[0m"
	@$(COMPILER) main.c dmx_output.c dmx_selector.c dmx_queue.c dmx_image.c dmx_devices.c dmx_channels.c $(SOURCES) -o dmxMidiCtrlNg $(FLAGS) $(LDFLAGS) 

.PHONY : clean all 

