#ifndef _DMX_FRAME_H
#define _DMX_FRAME_H

#include "dmx_defines.h"
#include "dmx_attr_colors.h"

enum {
	DMX_FRAME_IMAGE,
	DMX_FRAME_WAIT,
	DMX_FRAME_COMMAND,
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


struct dmx_frame_image
{
	unsigned int type;
	unsigned int dev_alloc;
	unsigned int dev_count;
	char **dev_names;

	unsigned int attr_alloc;
	unsigned int attr_count;
	union dmx_attr** attr_list;
};

struct dmx_frame_wait
{
	unsigned int type;
	unsigned int milis;
};

struct dmx_frame_command
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

union dmx_frame
{
	unsigned int type;
	struct dmx_frame_wait wait;
	struct dmx_frame_image image; 
	struct dmx_frame_command command; 
};





#endif
