#ifndef _DMX_FRAME_H
#define _DMX_FRAME_H

#include "dmx_defines.h"

enum {
	DMX_FRAME_IMAGE,
	DMX_FRAME_WAIT,
	DMX_FRAME_COMMAND,
};

struct dmx_img
{
	unsigned int dev_alloc;
	unsigned int dev_count;
	char **dev_names;

	unsigned int is_dim;
	unsigned int is_col;

	float dim;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	char* color;
};


struct dmx_frame_image
{
	unsigned int type;
	struct dmx_img* image;
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

typedef union
{
	unsigned int type;
	struct dmx_frame_wait wait;
	struct dmx_frame_image image; 
	struct dmx_frame_command command; 
} dmx_frame;

dmx_frame* dmx_frame_new(unsigned int type);
struct dmx_img* dmx_img_new(void);

void dmx_img_device_clear(struct dmx_img* image);
unsigned int dmx_img_device_count(struct dmx_img* image);
char* dmx_img_device_get(struct dmx_img* image,unsigned int idx);
void dmx_img_device_add(struct dmx_img* image, char* name);
void dmx_img_device_del(struct dmx_img* image, char* name);
unsigned int dmx_img_device_test(struct dmx_img* image, char* name);

void dmx_img_color_clear(struct dmx_img* image);
void dmx_img_color_setname(struct dmx_img* image, const char* colorname);
unsigned int dmx_img_color_test(struct dmx_img* image, const char* colorname);
void dmx_img_color_setrgb(struct dmx_img* image,unsigned int r, unsigned int g, unsigned int b);
const char* dmx_img_color_getname(struct dmx_img* image);
unsigned int dmx_img_color_getrgb(struct dmx_img* image,unsigned int* r,unsigned int* g, unsigned int* b);

void dmx_img_dim_clear(struct dmx_img* image);
void dmx_img_dim_set(struct dmx_img* image,float dim);
unsigned int dmx_img_dim_get(struct dmx_img* image,float *dim);


#endif
