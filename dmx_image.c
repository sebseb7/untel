#include <stdlib.h>
#include <string.h>

#include "dmx_image.h"

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
	unsigned int device_type;
	char device_name[DMX_NAME_LENGTH];
	unsigned int sets_alloc;
	unsigned int set_count;
	struct dmx_set* set_list;
};
*/

#define DMX_IMAGE_SETLIST_ALLOCATE_INITIAL 10

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

struct dmx_image* dmx_image_add(unsigned int type,char* name)
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

	image->device_type = type;
	strncpy(image->device_name,name,DMX_NAME_LENGTH);
	image->sets_alloc=DMX_IMAGE_SETLIST_ALLOCATE_INITIAL;
	image->set_count=0;
	image->set_list=malloc(sizeof(struct dmx_set)*DMX_IMAGE_SETLIST_ALLOCATE_INITIAL);
	
	return image;
}

void dmx_image_del(struct dmx_image* image)
{
	for(unsigned int i=0;i<images_inuse;i++)
	{
		if(image == dmx_image_list[i])
		{
			free(image->set_list);
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




void dmx_image_add_setCol(struct dmx_image* image,unsigned char red,unsigned char green,unsigned char blue)
{
	if(image->set_count == image->sets_alloc)
	{
		exit(EXIT_FAILURE);
	}
	image->set_list[image->set_count].attr_type=ATTR_TYPE_COLSET;
	image->set_list[image->set_count].color[0]=red;
	image->set_list[image->set_count].color[1]=green;
	image->set_list[image->set_count].color[2]=blue;
	image->set_count++;
}
void dmx_image_add_setDim(struct dmx_image* image,float value)
{
	if(image->set_count == image->sets_alloc)
	{
		exit(EXIT_FAILURE);
	}
	image->set_list[image->set_count].attr_type=ATTR_TYPE_DIM;
	image->set_list[image->set_count].dim=value;
	image->set_count++;
}
void dmx_image_add_setCode(struct dmx_image* image,void (*ref)(void))
{
	if(image->set_count == image->sets_alloc)
	{
		exit(EXIT_FAILURE);
	}
	image->set_list[image->set_count].attr_type=ATTR_TYPE_CODE;
	image->set_list[image->set_count].code=ref;
	image->set_count++;
}


void dmx_set_render(unsigned int type,char* name,struct dmx_set* set)
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
