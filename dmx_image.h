#ifndef _DMX_IMAGE_H
#define _DMX_IMAGE_H

#include "dmx_devices.h"

struct dmx_set
{
	unsigned int attr;
	unsigned int attr_type;
	float fvalue;
	unsigned int ivalue;
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
void dmx_image_add_setI(struct dmx_image* image,unsigned int attr,unsigned int value);
void dmx_image_add_setF(struct dmx_image* image,unsigned int attr,float value);
unsigned int dmx_image_get_count(void);
struct dmx_image* dmx_image_getbyidx(unsigned int index);

#endif
