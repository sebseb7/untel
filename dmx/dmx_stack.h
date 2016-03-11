#ifndef _DMX_STACK_H
#define _DMX_STACK_H

#include "dmx_defines.h"
#include "dmx_attr_colors.h"

enum {
	DMX_STACK_FRAME_IMAGE,
	DMX_STACK_FRAME_WAIT,
	DMX_STACK_FRAME_COMMAND,
};

struct dmx_attr_dim
{
	unsigned int type;
	float dim;
};

union dmx_attr
{
	unsigned int type;
	union dmx_attr_color color;
	struct dmx_attr_dim dim;
};


struct dmx_stack_frame_image
{
	unsigned int type;
	unsigned int dev_alloc;
	unsigned int dev_count;
	char **dev_names;

	unsigned int attr_alloc;
	unsigned int attr_count;
	union dmx_attr** attr_list;
};

struct dmx_stack_frame_wait
{
	unsigned int type;
	unsigned int milis;
};

struct dmx_stack_frame_command
{
	unsigned int type;
	unsigned int command;
	char *svalue1;
	char *svalue2;
	unsigned int ivalue1;
	unsigned int ivalue2;
	float fvalue1;
	float fvalue2;
};

union dmx_stack_frame
{
	unsigned int type;
	struct dmx_stack_frame_wait wait;
	struct dmx_stack_frame_image image; 
	struct dmx_stack_frame_command command; 
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
