UNAME := $(shell uname)

COMPILER = gcc

SOURCES=$(wildcard main.c queues/*.c queues/par56/*.c queues/strobe/*.c libs/*.c libs/mcugui/*.c libs/gui/*.c dmx/*.c)
DEPS   =$(patsubst %,.bin/%,$(SOURCES:.c=.d)) 
OBJECTS=$(patsubst %,.bin/%,$(SOURCES:.c=.o)) 

FLAGS= -O0 -g -I. -Ilibs -Idmx -I/usr/include/lua5.2 --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -Wextra -L.
LDFLAGS= -lm -lportmidi -lftdi1 -llo -llua5.2

FLAGS+=`sdl2-config --cflags`
LDFLAGS+=`sdl2-config --libs`

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
	@echo "  \033[1;34mCompile\033[0m"
	@$(COMPILER) $(OBJECTS) $(LDFLAGS)  -o dmxMidiCtrlNg

-include $(DEPS)

.bin/%.o: %.c Makefile
	@echo "  GCC $<"
	@mkdir -p $(dir $@)
	@$(COMPILER) $(FLAGS) -o $@ -c $<
	@$(COMPILER) $(FLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

gource:
	git log --reverse --pretty=format:"%at|%B" > git.log
	gource --caption-file git.log -a 1 --caption-duration 3 --caption-size 10 -max-files 99999 -disable-progress -stop-at-end  -user-scale 1 -highlight-all-users .
	rm git.log

.PHONY : clean all gource plot

