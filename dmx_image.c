#include <stdlib.h>
#include <string.h>

#include "dmx_image.h"

enum {
	ATTR_TYPE_INT,
	ATTR_TYPE_FLOAT
};

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

static struct dmx_image* dmx_image_list;

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
		return &dmx_image_list[index];
	}
	return NULL;
}

struct dmx_image * dmx_image_add(unsigned int type,char* name)
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
	struct dmx_image* image = &dmx_image_list[images_inuse];
	images_inuse++;

	image->device_type = type;
	strncpy(image->device_name,name,DMX_NAME_LENGTH);
	image->sets_alloc=DMX_IMAGE_SETLIST_ALLOCATE_INITIAL;
	image->set_count=0;
	image->set_list=malloc(sizeof(struct dmx_set)*DMX_IMAGE_SETLIST_ALLOCATE_INITIAL);

	return image;
}
void dmx_image_add_setI(struct dmx_image* image,unsigned int attr,unsigned int value)
{
	if(image->set_count == image->sets_alloc)
	{
		exit(EXIT_FAILURE);
	}
	image->set_list[image->set_count].attr=attr;
	image->set_list[image->set_count].attr_type=ATTR_TYPE_INT;
	image->set_list[image->set_count].ivalue=value;
	image->set_count++;
}
void dmx_image_add_setF(struct dmx_image* image,unsigned int attr,float value)
{
	if(image->set_count == image->sets_alloc)
	{
		exit(EXIT_FAILURE);
	}
	image->set_list[image->set_count].attr=attr;
	image->set_list[image->set_count].attr_type=ATTR_TYPE_FLOAT;
	image->set_list[image->set_count].fvalue=value;
	image->set_count++;
}
