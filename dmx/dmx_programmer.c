#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_defines.h"
#include "dmx_programmer.h"

#define DMX_PROGRAMMER_DEVICE_ALLOCATE_INITIAL 100

static char** dmx_programmer_device_list;

static unsigned int dmx_programmer_device_allocated=0;
static unsigned int dmx_programmer_device_inuse=0;


void dmx_programmer_device_clear(void)
{
	for(unsigned int i = 0;i < dmx_programmer_device_inuse;i++)
	{
		free(dmx_programmer_device_list[i]);
	}
	dmx_programmer_device_inuse = 0;
}

unsigned int dmx_progammer_device_count(void)
{
	return dmx_programmer_device_inuse;
}

void dmx_programmer_device_add(char* name)
{
	if(dmx_programmer_device_test(name))
		return;

	if(dmx_programmer_device_allocated==0)
	{
		dmx_programmer_device_list = malloc(sizeof(char*)*DMX_PROGRAMMER_DEVICE_ALLOCATE_INITIAL);
		dmx_programmer_device_allocated = DMX_PROGRAMMER_DEVICE_ALLOCATE_INITIAL;
	}

	if(dmx_programmer_device_inuse == dmx_programmer_device_allocated)
	{
		//realloc
		exit(EXIT_FAILURE);
	}

	dmx_programmer_device_list[dmx_programmer_device_inuse] = strndup(name,DMX_NAME_LENGTH);
	dmx_programmer_device_inuse++;
}

void dmx_programmer_device_del(char* name)
{
	for(unsigned int i = 0;i < dmx_programmer_device_inuse;i++)
	{
		if(strncmp(dmx_programmer_device_list[i],name,DMX_NAME_LENGTH)==0)
		{
			free(dmx_programmer_device_list[i]);
			
			if(i < (dmx_programmer_device_inuse-1))
			{
				dmx_programmer_device_list[i] = dmx_programmer_device_list[dmx_programmer_device_inuse-1];
			}
			
			dmx_programmer_device_inuse--;
		}
	}
}

unsigned int dmx_programmer_device_test(char* name)
{
	for(unsigned int i = 0;i < dmx_programmer_device_inuse;i++)
	{
		if(strncmp(dmx_programmer_device_list[i],name,DMX_NAME_LENGTH)==0)
		{
			return 1;
		}
	}
	return 0;
}



static unsigned int dmx_programmer_color_isset = 0;
static unsigned int dmx_programmer_color_r = 0;
static unsigned int dmx_programmer_color_g = 0;
static unsigned int dmx_programmer_color_b = 0;
static char* dmx_programmer_color_name = NULL;

void dmx_programmer_color_clear(void)
{
	dmx_programmer_color_isset=0;
	if(	dmx_programmer_color_name !=NULL)
	{
		free(dmx_programmer_color_name);
		dmx_programmer_color_name = NULL;
	}
}

void dmx_programmer_color_setbyname(const char* colorname)
{
	dmx_programmer_color_isset=1;
	
	if(	dmx_programmer_color_name !=NULL)
	{
		free(dmx_programmer_color_name);
		dmx_programmer_color_name = NULL;
	}

	dmx_programmer_color_name = strndup(colorname,DMX_NAME_LENGTH);
}

void dmx_programmer_color_setbyrgb(unsigned int r, unsigned int g, unsigned int b)
{
	dmx_programmer_color_r=r;
	dmx_programmer_color_g=g;
	dmx_programmer_color_b=b;
	dmx_programmer_color_isset=1;
	if(	dmx_programmer_color_name !=NULL)
	{
		free(dmx_programmer_color_name);
		dmx_programmer_color_name = NULL;
	}
}

unsigned int dmx_programmer_color_test(const char* colorname)
{
	if(dmx_programmer_color_isset)
	{
		if(dmx_programmer_color_name != NULL)
		{
			if(strncmp(dmx_programmer_color_name,colorname,DMX_NAME_LENGTH)==0)
			{
				return 1;
			}
		}
	}
	return 0;
}

unsigned int dmx_programmer_color_getrgb(unsigned int *r,unsigned int *g,unsigned int *b)
{
	if(dmx_programmer_color_isset)
	{
		*r = dmx_programmer_color_r;
		*g = dmx_programmer_color_g;
		*b = dmx_programmer_color_b;
		return 1;
	}
	return 0;
}

static unsigned int dmx_programmer_dim_isset = 0;
static float dmx_programmer_dim = 0.0f;

void dmx_programmer_dim_clear(void)
{
	dmx_programmer_dim_isset = 0;
}

void dmx_programmer_dim_set(float dim)
{
	dmx_programmer_dim = dim;
	dmx_programmer_dim_isset = 1;
}

unsigned int dmx_programmer_dim_get(float *dim)
{
	if(dmx_programmer_dim_isset)
	{
		*dim = dmx_programmer_dim;
		return 1;
	}
	return 0;
}


