#ifndef _DMX_STACK_H
#define _DMX_STACK_H

#include "dmx_devices.h"

enum {
	DMX_ATTR_COLOR,
	DMX_ATTR_FREQ,
	DMX_ATTR_DIM
};

enum {
	DMX_STACK_FRAME_IMAGE,
	DMX_STACK_FRAME_WAIT,
};


struct dmx_stack_frame_image
{
	unsigned int type;
	unsigned int dev_alloc;
	unsigned int dev_count;
	char **dev_names;

	unsigned int attr_alloc;
	unsigned int attr_count;
	struct dmx_attr** attr_list;
};

struct dmx_stack_frame_wait
{
	unsigned int type;
	unsigned int milis;
};

union dmx_stack_frame
{
	unsigned int type;
	struct dmx_stack_frame_wait wait;
	struct dmx_stack_frame_image image; 
};

struct dmx_stack 
{
	char category[DMX_NAME_LENGTH];
	char name[DMX_NAME_LENGTH];

	unsigned int active;

	unsigned int length;

	union dmx_stack_frame** frames;
};
	
unsigned int dmx_stack_get_count(void);
struct dmx_stack* dmx_stack_getbyidx(unsigned int index);

			

#endif
