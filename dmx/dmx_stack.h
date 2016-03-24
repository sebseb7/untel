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

	union dmx_frame** frames;
};

struct dmx_stack* dmx_stack_new(void);
unsigned int dmx_stack_get_count(void);
struct dmx_stack* dmx_stack_getbyidx(unsigned int index);

			

#endif
