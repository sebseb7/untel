#ifndef _DMX_IMAGE_H
#define _DMX_IMAGE_H

#include "dmx_devices.h"

struct dmx_set
{
	unsigned int attr_type;
	union {
		unsigned char color[3];
		float dim;
	};
};

struct dmx_image 
{
	unsigned int device_type;
	char device_name[DMX_NAME_LENGTH];
	unsigned int sets_alloc;
	unsigned int set_count;
	struct dmx_set* set_list;
};


struct dmx_image * dmx_image_add(unsigned int type,char* name);
void dmx_image_add_setCol(struct dmx_image* image,unsigned char red,unsigned char green,unsigned char blue);
void dmx_image_add_setDim(struct dmx_image* image,float value);
unsigned int dmx_image_get_count(void);
struct dmx_image* dmx_image_getbyidx(unsigned int index);

void dmx_set_render(unsigned int type,char* name,struct dmx_set* set);

#endif
