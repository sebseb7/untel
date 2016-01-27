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

struct dmx_image * dmx_image_add(unsigned int type,char* name)
{
	struct dmx_image* image = malloc(sizeof(*image));

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
