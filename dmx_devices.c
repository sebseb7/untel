#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_devices.h"
#include "dmx_channels.h"

/*struct dmx_device {
	unsigned int type;
	unsigned int addr;
	char name[DMX_NAME_LENGTH];
	void* device;
};

struct dmx_device_ledpar {
	unsigned int addr;
	unsigned int type;
	float red;
	float green;
	float blue;
	float dim;
	unsigned int blackout;
};
*/

#define DMX_DEVICE_ALLOCATE_INITIAL 100
#define DMX_DEVICE_ALLOCATE_STEP 10

static struct dmx_device* dmx_device_list;

static unsigned int devices_allocated=0;
static unsigned int devices_inuse=0;

static void dmx_device_add(void* device,unsigned int addr,unsigned int type,char* name)
{
	if(0==devices_allocated)
	{
		dmx_device_list = malloc(sizeof(*dmx_device_list)*DMX_DEVICE_ALLOCATE_INITIAL);
		devices_allocated=DMX_DEVICE_ALLOCATE_INITIAL;
	}

	if(devices_inuse == devices_allocated)
	{
		//implement realloc
		printf("f1\n");
		exit(EXIT_FAILURE);
	}
	strncpy(dmx_device_list[devices_inuse].name,name,DMX_NAME_LENGTH);
	dmx_device_list[devices_inuse].type=type;
	dmx_device_list[devices_inuse].addr=addr;
	dmx_device_list[devices_inuse].device=device;
	dmx_device_list[devices_inuse].refcount=1;

	devices_inuse++;
}

unsigned int dmx_get_device_count(void)
{
	return devices_inuse;
}
void dmx_devices_free(void)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if(dmx_device_list[i].type == DMX_DEVICE_LEDPAR6)
		{
			struct dmx_device_ledpar6* ledpar = dmx_device_list[i].device;
			free(ledpar);
		}
	}
	devices_inuse=0;
	devices_allocated=0;
	free(dmx_device_list);
}
void dmx_devices_clear(void)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if(dmx_device_list[i].type == DMX_DEVICE_LEDPAR6)
		{
			struct dmx_device_ledpar6* ledpar = dmx_device_list[i].device;
			ledpar->red = 0;
			ledpar->green = 0;
			ledpar->blue = 0;
			ledpar->dim = 0.0f;
			ledpar->blackout = 0;
		}
	}
}
struct dmx_device* dmx_get_device(unsigned int type,char* name)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if((dmx_device_list[i].type==type)&&(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH)))
		{
			return &dmx_device_list[i];
		}
	}
	return NULL;
}
struct dmx_device* dmx_get_device_byidx(unsigned int index)
{
	if(index < devices_inuse)
	{
		return &dmx_device_list[index];
	}
	return NULL;
}


struct dmx_device_ledpar6* dmx_device_create_ledpar6(unsigned int addr,char* name)
{
	if(devices_inuse>0)
	{
		for(unsigned int i=0;i<devices_inuse;i++)
		{
			if(
				(dmx_device_list[i].type==DMX_DEVICE_LEDPAR6)&&
				(dmx_device_list[i].addr==addr)&&
				(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH))
			)
			{
					dmx_device_list[i].refcount++;
					return dmx_device_list[i].device;
			}
		}
	}
	
	struct dmx_device_ledpar6* ledpar = malloc(sizeof(*ledpar));
	
	ledpar->red = 0;
	ledpar->green = 0;
	ledpar->blue = 0;
	ledpar->dim = 0.0f;
	ledpar->blackout = 0;

	dmx_device_add(ledpar,addr,DMX_DEVICE_LEDPAR6,name);

	return ledpar;
}
//void dmx_device_create_fog(unsigned int addr);
//void dmx_device_create_miniscanhpe(unsigned int addr);


void dmx_device_render_ledpar6(struct dmx_device* device)
{
	struct dmx_device_ledpar6* ledpar = device->device;

	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;

	red = ledpar->red*ledpar->dim;
	green = ledpar->green*ledpar->dim;
	blue = ledpar->blue*ledpar->dim;

	dmx_channel_set(device->addr,red);
	dmx_channel_set(device->addr+1,green);
	dmx_channel_set(device->addr+2,blue);

	dmx_channel_set(device->addr+3,0);
	dmx_channel_set(device->addr+4,0);
	dmx_channel_set(device->addr+5,0);


}

void dmx_device_render_ledpar6_sdl(struct dmx_device* device,unsigned int* pixelbuffer,unsigned int row, unsigned int col)
{
	
	struct dmx_device_ledpar6* ledpar = device->device;

	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;

	red = ledpar->red*ledpar->dim;
	green = ledpar->green*ledpar->dim;
	blue = ledpar->blue*ledpar->dim;

	unsigned int color = (red<<16)+(green<<8)+blue;

	if(pixelbuffer[((row*30)*300)+(col*30)] != color)
	{
		for(unsigned int i=0;i<30;i++)
		{
			for(unsigned int j=0;j<30;j++) 
			{
				pixelbuffer[(((row*30)+j)*300)+(col*30)+i] = color;
			}
		}
	}
}
