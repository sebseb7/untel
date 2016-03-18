#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_frame.h"

#define DMX_IMG_DEVICE_ALLOCATE_INITIAL 100

dmx_frame* dmx_frame_new(unsigned int type)
{
	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type = type;

	if(type == DMX_FRAME_IMAGE)
	{
		struct dmx_img* image = malloc(sizeof(struct dmx_img));

		image->dev_alloc=0;
		image->dev_count=0;
		image->dev_names=NULL;

		image->is_dim=0;
		image->is_col=0;
		image->dim=0.0f;
		image->r=0;
		image->g=0;
		image->b=0;
		image->color=NULL;
		frame->image.image = image;
	}
	else if(type == DMX_FRAME_WAIT)
	{
		frame->wait.milis = 0;
	}
	else if(type == DMX_FRAME_COMMAND)
	{
		frame->command.command = 0;
		frame->command.svalue1 = NULL;
		frame->command.svalue2 = NULL;
		frame->command.ivalue1=0;
		frame->command.ivalue2=0;
		frame->command.fvalue1=0.0f;
		frame->command.fvalue2=0.0f;
	}
	
	return frame;
}

struct dmx_img* dmx_img_new(void)
{
	struct dmx_img* image = malloc(sizeof(struct dmx_img));

	image->dev_alloc=0;
	image->dev_count=0;
	image->dev_names=NULL;

	image->is_dim=0;
	image->is_col=0;
	image->dim=0.0f;
	image->r=0;
	image->g=0;
	image->b=0;
	image->color=NULL;

	return image;
}

void dmx_img_device_add(struct dmx_img* image,char* name)
{
	if(dmx_img_device_test(image,name))
		return;

	if(image->dev_alloc==0)
	{
		image->dev_names = malloc(sizeof(char*)*DMX_IMG_DEVICE_ALLOCATE_INITIAL);
		image->dev_alloc = DMX_IMG_DEVICE_ALLOCATE_INITIAL;
	}

	if(image->dev_count == image->dev_alloc)
	{
		//realloc
		exit(EXIT_FAILURE);
	}

	image->dev_names[image->dev_count] = strndup(name,DMX_NAME_LENGTH);
	image->dev_count++;
}

void dmx_img_device_del(struct dmx_img* image,char* name)
{
	for(unsigned int i = 0;i < image->dev_count;i++)
	{
		if(strncmp(image->dev_names[i],name,DMX_NAME_LENGTH)==0)
		{
			free(image->dev_names[i]);
			
			if(i < (image->dev_count - 1))
			{
				image->dev_names[i] = image->dev_names[image->dev_count - 1];
			}
			
			image->dev_count--;
		}
	}
}


unsigned int dmx_img_device_test(struct dmx_img* image,char* name)
{
	for(unsigned int i = 0;i < image->dev_count;i++)
	{
		if(strncmp(image->dev_names[i],name,DMX_NAME_LENGTH)==0)
		{
			return 1;
		}
	}
	return 0;
}

void dmx_img_color_setname(struct dmx_img* image,const char* colorname)
{
	dmx_img_color_clear(image);
	image->is_col =  DMX_ATTR_COLOR_NAME;
	image->color = strndup(colorname,DMX_NAME_LENGTH);
}

void dmx_img_color_setrgb(struct dmx_img* image,unsigned int r, unsigned int g, unsigned int b)
{
	if(r & 0xff00) r=0xff;
	if(g & 0xff00) g=0xff;
	if(b & 0xff00) b=0xff;

	dmx_img_color_clear(image);
	image->is_col =  DMX_ATTR_COLOR_RGB;
	image->r=r;
	image->g=g;
	image->b=b;
}

unsigned int dmx_img_color_getrgb(struct dmx_img* image,unsigned int *r,unsigned int *g,unsigned int *b)
{
	if(image->is_col == DMX_ATTR_COLOR_RGB)
	{
		*r = image->r;
		*g = image->g;
		*b = image->b;
		return 1;
	}
	return 0;
}


unsigned int dmx_img_color_test(struct dmx_img* image,const char* colorname)
{
	if(image->is_col == DMX_ATTR_COLOR_NAME)
	{
		if(image->color != NULL)
		{
			if(strncmp(image->color,colorname,DMX_NAME_LENGTH)==0)
			{
				return 1;
			}
		}
	}
	return 0;
}
void dmx_img_color_clear(struct dmx_img* image)
{
	image->is_col = 0;
	image->r = 0;
	image->g = 0;
	image->b = 0;
	if(	image->color !=NULL)
	{
		free(image->color);
		image->color = NULL;
	}
}

void dmx_img_dim_clear(struct dmx_img* image)
{
	image->is_dim=0;
	image->dim=0.0f;
}

void dmx_img_dim_set(struct dmx_img* image,float dim)
{
	if(dim < 0.0f) dim=0.0f;
	if(dim > 1.0f) dim=1.0f;

	image->dim = dim;
	image->is_dim = 1;
}

unsigned int dmx_img_dim_get(struct dmx_img* image,float *dim)
{
	if(image->is_dim)
	{
		*dim = image->dim;
		return 1;
	}
	return 0;
}


