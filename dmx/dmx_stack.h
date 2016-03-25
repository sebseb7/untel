#ifndef _DMX_STACK_H
#define _DMX_STACK_H

#include "dmx_defines.h"
#include "dmx_frame.h"


struct dmx_stack 
{
	char category[DMX_NAME_LENGTH];
	char name[DMX_NAME_LENGTH];

	unsigned int active;

	unsigned int length;
	unsigned int alloc;

	dmx_frame** frames;
};

struct dmx_stack* dmx_stack_new(void);
unsigned int dmx_stack_get_count(void);
struct dmx_stack* dmx_stack_getbyidx(unsigned int index);

void dmx_stack_add_imgframe(struct dmx_stack* stack,struct dmx_img* img);
unsigned int dmx_stack_frame_count(struct dmx_stack* stack);			

#endif
