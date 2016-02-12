#ifndef _DMX_IMAGE_H
#define _DMX_IMAGE_H

#include "dmx_devices.h"

enum {
	ATTR_TYPE_COLSET,
	ATTR_TYPE_CODE,
	ATTR_TYPE_DIM
};


struct dmx_set
{
	unsigned int attr_type;
	union {
		unsigned char color[3];
		float dim;
		void (*code)(void);
	};
};

struct dmx_image 
{
	unsigned int active;

	unsigned int priority;

	unsigned int dev_alloc;
	unsigned int dev_count;
	unsigned int* dev_types;
	void* dev_names;

	unsigned int set_alloc;
	unsigned int set_count;
	struct dmx_set** set_list;

	unsigned int selector_alloc;
	unsigned int selector_count;
	char** selector_names;
	char** selector_pos;
	unsigned int* selector_blendpulse;
	unsigned int* selector_blendstart;
	char** selector_blendtopos;
};
	
struct dmx_image * dmx_image_new(unsigned int priority);
struct dmx_set* dmx_set_new_dim(float dim);
void dmx_image_attach_set(struct dmx_image* image,struct dmx_set* set);
void dmx_image_add_device(struct dmx_image* image,unsigned int type,char* name);
void dmx_image_set_selector(struct dmx_image* image,char* name, char* pos);
void dmx_image_blend_selector(struct dmx_image* image,char* name, char* pos,unsigned int beatPulses);
void dmx_image_del(struct dmx_image* image);
void dmx_image_show(struct dmx_image* image);
void dmx_image_hide(struct dmx_image* image);
unsigned int dmx_image_get_count(void);
struct dmx_image* dmx_image_getbyidx(unsigned int index);

			
void dmx_image_render(struct dmx_image* image,unsigned int beatpulse);

#endif
