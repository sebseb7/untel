#include <stdlib.h>
#include <string.h>

#include "dmx_devices.h"

/*struct dmx_device {
	unsigned int type;
	unsigned int addr;
	void* device;
};

struct dmx_device_ledpar {
	unsigned int addr;
	unsigned int type;
	char name[DMX_NAME_LENGTH];
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

static void dmx_device_add(void* device,unsigned int addr,unsigned int type)
{
	if(0==devices_allocated)
	{
		dmx_device_list = malloc(sizeof(*dmx_device_list)*DMX_DEVICE_ALLOCATE_INITIAL);
		devices_allocated=DMX_DEVICE_ALLOCATE_INITIAL;
	}

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
struct dmx_device* dmx_get_device(unsigned int index)
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
			if((dmx_device_list[i].type==DMX_DEVICE_LEDPAR)&&(dmx_device_list[i].addr==addr))
			{
				dmx_device_list[i].refcount++;
				return dmx_device_list[i].device;
			}
		}
	}
	struct dmx_device_ledpar* ledpar = malloc(sizeof(*ledpar));

	strncpy(ledpar->name,name,DMX_NAME_LENGTH);
	ledpar->addr = addr;
	ledpar->type = type;
	ledpar->red = 0.0f;
	ledpar->green = 0.0f;
	ledpar->blue = 0.0f;
	ledpar->dim = 0.0f;
	ledpar->blackout = 0;

	dmx_device_add(ledpar,addr,DMX_DEVICE_LEDPAR);

	return ledpar;
}
//void dmx_device_create_fog(unsigned int addr);


