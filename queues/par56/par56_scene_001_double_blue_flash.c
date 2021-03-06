#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"
#include "par56.h"

static struct dmx_image* images[PAR56_COUNT];

static void init(void)
{
	create_par56();
		
	for(int i = 0;i < PAR56_COUNT;i++) images[i] = dmx_image_new(0);
	dmx_image_add_device(images[0],DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(images[1],DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(images[2],DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(images[3],DMX_DEVICE_LEDPAR6,"par-4");
	for(int i = 0;i < PAR56_COUNT;i++)
	{
		dmx_image_set_selector(images[i],"LP COL","blue");
		dmx_image_show(images[i]);
	}

}

static unsigned int step = 0;
static unsigned int map = (1<<0)|(1<<1)|(1<<2)|(1<<3);
static void deinit(void)
{
	for(int i = 0;i < PAR56_COUNT;i++)
		dmx_image_del(images[i]);
	step=0;
	map = (1<<0)|(1<<1)|(1<<2)|(1<<3);
}

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	unsigned int bar = 0;
	switch(step++)
	{
		case 0:
			dmx_globvar_get("barcnt",&bar);
			unsigned int substep = bar % 6;

			map=0;
			if(substep == 0)
			{
				map = (1<<0)|(1<<1)|(1<<2)|(1<<3);
			}
			else if(substep == 1)
			{
				map = (1<<0)|(1<<3);	
			}
			else if(substep == 2)
			{
				map = (1<<1)|(1<<2);
			}
			else if(substep == 3)
			{
				map = (1<<0)|(1<<3);	
			}
			else if(substep == 4)
			{
				map = (1<<1)|(1<<2);
			}
			else if(substep == 5)
			{
				map = (1<<0)|(1<<3);	
			}


			for(int i = 0;i < PAR56_COUNT;i++)
				if(map & (1<<i))dmx_image_set_selector(images[i],"LP DIM","on");
			return 20;
		case 1:
			for(int i = 0;i < PAR56_COUNT;i++)
				if(map & (1<<i))dmx_image_set_selector(images[i],"LP DIM","off");
			return 700;
		case 2:
			for(int i = 0;i < PAR56_COUNT;i++)
				if(map & (1<<i))dmx_image_set_selector(images[i],"LP DIM","on");
			return 20;
		default:
			for(int i = 0;i < PAR56_COUNT;i++)
				if(map & (1<<i))dmx_image_set_selector(images[i],"LP DIM","off");
			step=0;
			return 3000;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-001",init,deinit,tick);

}
