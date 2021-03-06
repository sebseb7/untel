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

struct dmx_image * dmx_image_new(unsigned int priority)
{
	if(0==images_allocated)
	{
		dmx_image_list = malloc(sizeof(*dmx_image_list)*DMX_IMAGE_ALLOCATE_INITIAL);
		images_allocated=DMX_IMAGE_ALLOCATE_INITIAL;
	}

	if(images_inuse == images_allocated)
	{
		//implement realloc
		printf("f2\n");
		exit(EXIT_FAILURE);
	}
	struct dmx_image* image = malloc(sizeof(*image));
	dmx_image_list[images_inuse]=image;
	images_inuse++;
	
	image->active=0;

	image->priority=priority;

	image->dev_alloc=DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL;
	image->dev_count=0;
	image->dev_types=malloc(sizeof(unsigned int)*DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL);
	image->dev_names=malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_DEVICELIST_ALLOCATE_INITIAL);

	image->set_alloc=DMX_IMAGE_SETLIST_ALLOCATE_INITIAL;
	image->set_count=0;
	image->set_list=malloc(sizeof(struct dmx_set*)*DMX_IMAGE_SETLIST_ALLOCATE_INITIAL);

	image->selector_alloc=DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL;
	image->selector_count=0;
	image->selector_names=     malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	image->selector_pos=       malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	image->selector_blendtopos=malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	image->selector_blendpulse=malloc(sizeof(unsigned int)*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	image->selector_blendstart=malloc(sizeof(unsigned int)*DMX_IMAGE_SELECTORLIST_ALLOCATE_INITIAL);
	
	return image;
}
void dmx_image_add_device(struct dmx_image* image,unsigned int type,char* name)
{
	unsigned int index = image->dev_count;

	if(image->dev_count < image->dev_alloc)
	{
		image->dev_types[index]=type;
		strncpy((char *)(image->dev_names+(sizeof(char)*index*DMX_NAME_LENGTH)),name,DMX_NAME_LENGTH);

		image->dev_count++;
	}

}
void dmx_image_set_selector(struct dmx_image* image,char* name, char* pos)
{
	for(unsigned int i=0;i<image->selector_count;i++)
	{
		if(0==strncmp((char*)(image->selector_names +(i*DMX_NAME_LENGTH)),name,DMX_NAME_LENGTH))
		{
			strncpy((char*)(image->selector_pos + (i*DMX_NAME_LENGTH)), pos, DMX_NAME_LENGTH);
			image->selector_blendpulse[i]=0;
			image->selector_blendstart[i]=0;
			return;
		}
	}

	
	unsigned int index = image->selector_count;
	if(image->selector_count < image->selector_alloc)
	{
		strncpy((char*)(image->selector_names + (index*DMX_NAME_LENGTH)),name,DMX_NAME_LENGTH);
		strncpy((char*)(image->selector_pos +(index*DMX_NAME_LENGTH)),  pos, DMX_NAME_LENGTH);
		image->selector_blendpulse[index]=0;
		image->selector_blendstart[index]=0;
		image->selector_count++;
	}
}
void dmx_image_blend_selector(struct dmx_image* image,char* name, char* pos,unsigned int beatPulses)
{
	for(unsigned int i=0;i<image->selector_count;i++)
	{
		if(0==strncmp((char*)(image->selector_names + (i*DMX_NAME_LENGTH)),name,DMX_NAME_LENGTH))
		{
			if(image->selector_blendpulse[i] != 0)
			{
				strncpy((char*)(image->selector_pos + (i*DMX_NAME_LENGTH)), (char*)(image->selector_blendtopos + (i*DMX_NAME_LENGTH)), DMX_NAME_LENGTH);
			}
			//printf("blend start (%s)\n",pos);
			strncpy((char*)(image->selector_blendtopos + (i*DMX_NAME_LENGTH)), pos, DMX_NAME_LENGTH);
			image->selector_blendpulse[i]=beatPulses;
			image->selector_blendstart[i]=0;
			return;
		}
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
			free(image->selector_blendtopos);
			free(image->selector_blendpulse);
			free(image->selector_blendstart);
			free(image);

			if(i != (images_inuse-1))
			{
				dmx_image_list[i]=dmx_image_list[images_inuse-1];
			}
			images_inuse--;
			if(0==images_inuse)
			{
				free(dmx_image_list);
				dmx_image_list = NULL;
				images_allocated=0;
			}
			return;
		}
	}
}

void dmx_image_show(struct dmx_image* image)
{
	image->active=1;
}
void dmx_image_hide(struct dmx_image* image)
{
	image->active=0;
}

static void dmx_set_render(unsigned int type,char* name,struct dmx_set* set)
{
	if(type == DMX_DEVICE_LEDPAR6)
	{
		struct dmx_device_ledpar6* ledpar = dmx_get_device(DMX_DEVICE_LEDPAR6, name)->device;
	
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
	else if(type == DMX_DEVICE_STROBE)
	{
		struct dmx_device_strobe* strobe = dmx_get_device(DMX_DEVICE_STROBE, name)->device;
	
		if(set->attr_type==ATTR_TYPE_DIM)
		{
			strobe->dim=set->dim;
		}
		else if(set->attr_type==ATTR_TYPE_FREQ)
		{
			strobe->freq=set->freq;
		}
		else if(set->attr_type==ATTR_TYPE_CODE)
		{
			set->code();
		}
	}

}

static void dmx_set_render_blend(unsigned int type,char* name,struct dmx_set* set,unsigned int pct)
{
	if(type == DMX_DEVICE_LEDPAR6)
	{
		struct dmx_device_ledpar6* ledpar = dmx_get_device(DMX_DEVICE_LEDPAR6, name)->device;
	
		if(set->attr_type==ATTR_TYPE_COLSET)
		{
			ledpar->red= ((set->color[0]*pct)/100.0f)+((ledpar->red*(100-pct)/100.0f));
			ledpar->green= ((set->color[1]*pct)/100.0f)+((ledpar->green*(100-pct)/100.0f));
			ledpar->blue= ((set->color[2]*pct)/100.0f)+((ledpar->blue*(100-pct)/100.0f));
		}
		else if(set->attr_type==ATTR_TYPE_DIM)
		{
			ledpar->dim= ((set->dim*pct)/100.0f)+((ledpar->dim*(100-pct)/100.0f));
		}
		else if(set->attr_type==ATTR_TYPE_CODE)
		{
			set->code();
		}
	}
	else if(type == DMX_DEVICE_STROBE)
	{
		struct dmx_device_strobe* ledpar = dmx_get_device(DMX_DEVICE_STROBE, name)->device;
	
		if(set->attr_type==ATTR_TYPE_DIM)
		{
			ledpar->dim= ((set->dim*pct)/100.0f)+((ledpar->dim*(100-pct)/100.0f));
		}
		else if(set->attr_type==ATTR_TYPE_FREQ)
		{
			ledpar->freq=((set->freq*pct)/100.0f)+((ledpar->freq*(100-pct)/100.0f));
		}
		else if(set->attr_type==ATTR_TYPE_CODE)
		{
			set->code();
		}
	}

}
struct dmx_set* dmx_set_new_dim(float dim)
{
	struct dmx_set* set1 = malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_DIM;
	set1->dim=dim;

	return set1;
}
struct dmx_set* dmx_set_new_freq(float freq)
{
	struct dmx_set* set1 = malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_FREQ;
	set1->freq=freq;

	return set1;
}
void dmx_image_attach_set(struct dmx_image* image,struct dmx_set* set)
{
	unsigned int index = image->set_count;

	if(image->set_count < image->set_alloc)
	{
		image->set_list[index]=set;
		image->set_count++;
	}
}

void dmx_image_render(struct dmx_image* image,unsigned int beatpulse)
{
	//printf("render image\n");
	if(image->active == 0)
		return;

	for(unsigned int i=0;i<image->dev_count;i++)
	{
		for(unsigned int j=0;j<image->set_count;j++)
		{
			dmx_set_render(image->dev_types[i],(char*)(image->dev_names + (sizeof(char)*i*DMX_NAME_LENGTH)),image->set_list[j]);
		}

		for(unsigned int j=0;j<image->selector_count;j++)
		{
			struct dmx_selector* selector = dmx_selector_getbyname((char*)(image->selector_names + (j*DMX_NAME_LENGTH)));

			unsigned int blendpct=0;

			if(image->selector_blendpulse[j] != 0)
			{
				if(image->selector_blendstart[j]==0)
				{
					image->selector_blendstart[j]=beatpulse;
				}

				unsigned int start = image->selector_blendstart[j];
				unsigned int duration = image->selector_blendpulse[j];

				if(beatpulse > (start+duration))
				{
					image->selector_blendstart[j]=0;
					image->selector_blendpulse[j]=0;
					strncpy((char*)(image->selector_pos + (j*DMX_NAME_LENGTH)), (char*)(image->selector_blendtopos + (j*DMX_NAME_LENGTH)) , DMX_NAME_LENGTH);
					//printf("blend done (%s)\n",(char*)&image->selector_pos[j*DMX_NAME_LENGTH]);
				}
				else
				{
					blendpct = (100*(beatpulse-start))/duration;
					if(blendpct > 100)
						blendpct = 100;

					//printf("pct: %i %i        \n",blendpct,beatpulse-start);
				}

			}
			
			if(blendpct > 0)
			{
				unsigned int pos = selector->getidbyname((char*)(image->selector_pos + (j*DMX_NAME_LENGTH)));
				unsigned int posblendto = selector->getidbyname((char*)(image->selector_blendtopos + (j*DMX_NAME_LENGTH)));
				
				
				selector->render(pos);

				for(unsigned int k=0;k<selector->set_count;k++)
				{
					dmx_set_render(image->dev_types[i],(char*)(image->dev_names + (sizeof(char)*i*DMX_NAME_LENGTH)),selector->set_list[k]);
				}
				selector->render(posblendto);

				for(unsigned int k=0;k<selector->set_count;k++)
				{
					dmx_set_render_blend(image->dev_types[i],(char*)(image->dev_names + (sizeof(char)*i*DMX_NAME_LENGTH)),selector->set_list[k],blendpct);
				}
			}
			else
			{
				unsigned int pos = selector->getidbyname((char*)(image->selector_pos + (j*DMX_NAME_LENGTH)));
	
				selector->render(pos);

				for(unsigned int k=0;k<selector->set_count;k++)
				{
					//printf("render image (%s) (%s) (%s)\n",(char*)&image->dev_names[i*DMX_NAME_LENGTH],(char*)&image->selector_names[j*DMX_NAME_LENGTH],(char*)&image->selector_pos[j*DMX_NAME_LENGTH]);


					dmx_set_render(image->dev_types[i],(char*)(image->dev_names + (sizeof(char)*i*DMX_NAME_LENGTH)),selector->set_list[k]);
				}
			}

		}
	}
}

