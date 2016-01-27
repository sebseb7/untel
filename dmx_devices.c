#include <stdlib.h>
#include <string.h>

#include "dmx_devices.h"

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
struct dmx_device* dmx_get_device(unsigned int type,char* name)
{
	for(unsigned int i=0;i<devices_inuse;i++)
	{
		if((dmx_device_list[i].type==type)&&(strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH)))
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


struct dmx_device_ledpar* dmx_device_create_ledpar(unsigned int addr,unsigned int type,char* name)
{
	if(devices_inuse>0)
	{
		for(unsigned int i=0;i<devices_inuse;i++)
		{
			if(
				(dmx_device_list[i].type==DMX_DEVICE_LEDPAR)&&
				(dmx_device_list[i].addr==addr)&&
				(0==strncmp(dmx_device_list[i].name,name,DMX_NAME_LENGTH))&&
				(((struct dmx_device_ledpar*) dmx_device_list[i].device)->type==type)
			)
			{
					dmx_device_list[i].refcount++;
					return dmx_device_list[i].device;
			}
		}
	}
	
	struct dmx_device_ledpar* ledpar = malloc(sizeof(*ledpar));

	ledpar->addr = addr;
	ledpar->type = type;
	ledpar->red = 0.0f;
	ledpar->green = 0.0f;
	ledpar->blue = 0.0f;
	ledpar->dim = 0.0f;
	ledpar->blackout = 0;

	dmx_device_add(ledpar,addr,DMX_DEVICE_LEDPAR,name);

	return ledpar;
}
//void dmx_device_create_fog(unsigned int addr);


