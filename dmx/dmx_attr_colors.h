#ifndef _DMX_ATTR_COLORS_H
#define _DMX_ATTR_COLORS_H

#include "dmx_defines.h"

enum {
	DMX_ATTR_COLOR_NAME,
	DMX_ATTR_COLOR_RGB,
};


struct dmx_attr_color_rgb
{
	unsigned int type;
	unsigned int r;
	unsigned int g;
	unsigned int b;
};

struct dmx_attr_color_name
{
	unsigned int type;
	char name[DMX_NAME_LENGTH];
};

union dmx_attr_color
{
	unsigned int type;
	struct dmx_attr_color_name name;
	struct dmx_attr_color_rgb rgb; 
};

unsigned int dmx_attr_colors_get_count(void);
const char* dmx_attr_colors_get_name(unsigned int idx);

#endif

