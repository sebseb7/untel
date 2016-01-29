#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_image.h"
#include "dmx_selector.h"

/*
struct dmx_set
{
	unsigned int attr;
	unsigned int attr_type;
	float fvalue;
	unsigned int ivalue;
};

struct dmx_image 
{
	unsigned int dev_alloc;
	unsigned int dev_count;
	unsigned int* dev_types;
	unsigned char** dev_names;

	unsigned int selector_alloc;
	unsigned int selector_count;
	unsigned char** selector_names;
	unsigned char** selector_pos;
};
*/

#define DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL 10
#define DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL 10

#define DMX_IMAGE_ALLOCATE_INITIAL 100
#define DMX_IMAGE_ALLOCATE_STEP 10

static struct dmx_image** dmx_image_list;

static unsigned int images_allocated=0;
static unsigned int images_inuse=0;

unsigned int dmx_image_get_count(void)
{
	return images_inuse;
}
struct dmx_image* dmx_image_getbyidx(unsigned int index)
{
	if(index < images_inuse)
	{
		return dmx_image_list[index];
	}
	return NULL;
}

struct dmx_image * dmx_image_new(void)
{
	if(0==images_allocated)
	{
		dmx_image_list = malloc(sizeof(*dmx_image_list)*DMX_IMAGE_ALLOCATE_INITIAL);
		images_allocated=DMX_IMAGE_ALLOCATE_INITIAL;
	}

	if(images_inuse == images_allocated)
	{
		//implement realloc
		exit(EXIT_FAILURE);
	}
	struct dmx_image* image = malloc(sizeof(*image));
	dmx_image_list[images_inuse]=image;
	images_inuse++;

	image->dev_alloc=DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL;
	image->dev_count=0;
	image->dev_types=malloc(sizeof(unsigned int)*DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL);
	image->dev_names=malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL);

	image->selector_alloc=DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL;
	image->selector_count=0;
	image->selector_names=malloc(sizeof(unsigned int)*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	image->selector_pos=malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	
	return image;
}
void dmx_image_add_device(struct dmx_image* image,unsigned int type,char* name)
{
	unsigned int index = image->dev_count;

	if(image->dev_count < image->dev_alloc)
	{
		image->dev_types[index]=type;
		strncpy((char*)&image->dev_names[index*DMX_NAME_LENGTH],name,DMX_NAME_LENGTH);

		image->dev_count++;
	}

}
void dmx_image_add_selector(struct dmx_image* image,char* name, char* pos)
{
	unsigned int index = image->selector_count;

	if(image->selector_count < image->selector_alloc)
	{
		strncpy((char*)&image->selector_names[index*DMX_NAME_LENGTH],name,DMX_NAME_LENGTH);
		strncpy((char*)&image->selector_pos[index*DMX_NAME_LENGTH],  pos, DMX_NAME_LENGTH);

		image->selector_count++;
	}
}

void dmx_image_del(struct dmx_image* image)
{
	for(unsigned int i=0;i<images_inuse;i++)
	{
		if(image == dmx_image_list[i])
		{
			free(image->dev_types);
			free(image->dev_names);
			free(image->selector_names);
			free(image->selector_pos);
			free(image);

			if(i != (images_inuse-1))
			{
				dmx_image_list[i]=dmx_image_list[images_inuse-1];
			}
			images_inuse--;
			return;
		}
	}
}





static void dmx_set_render(unsigned int type,char* name,struct dmx_set* set)
{
	if(type == DMX_DEVICE_LEDPAR)
	{
		struct dmx_device_ledpar* ledpar = dmx_get_device(DMX_DEVICE_LEDPAR, name)->device;
	
		if(set->attr_type==ATTR_TYPE_COLSET)
		{
			ledpar->red=set->color[0];
			ledpar->green=set->color[1];
			ledpar->blue=set->color[2];
		}
		else if(set->attr_type==ATTR_TYPE_DIM)
		{
			ledpar->dim=set->dim;
		}
		else if(set->attr_type==ATTR_TYPE_CODE)
		{
			set->code();
		}
	}

}

void dmx_image_render(struct dmx_image* image)
{
	//printf("render image\n");

	for(unsigned int i=0;i<image->dev_count;i++)
	{
		for(unsigned int j=0;j<image->selector_count;j++)
		{
			struct dmx_selector* selector = dmx_selector_getbyname((char*)&image->selector_names[j*DMX_NAME_LENGTH]);	
	
			unsigned int pos = selector->getidbyname((char*)&image->selector_pos[j*DMX_NAME_LENGTH]);
	
			selector->render(pos);

			for(unsigned int k=0;k<selector->set_count;k++)
			{
				//printf("render image (%s) (%s) (%s)\n",(char*)&image->dev_names[i*DMX_NAME_LENGTH],(char*)&image->selector_names[j*DMX_NAME_LENGTH],(char*)&image->selector_pos[j*DMX_NAME_LENGTH]);


				dmx_set_render(image->dev_types[i],(char*)&image->dev_names[i*DMX_NAME_LENGTH],selector->set_list[k]);
			}
		}
	}
}

