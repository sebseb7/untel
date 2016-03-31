#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"
#include "par56.h"


static struct dmx_image* images[PAR56_COUNT];
static struct dmx_set* sets[PAR56_COUNT];

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
		sets[i] = dmx_set_new_dim(0.0f);
		dmx_image_attach_set(images[i],sets[i]);
		dmx_image_show(images[i]);
	}

}

static unsigned int step = 0;
static void deinit(void)
{
	for(int i = 0;i < PAR56_COUNT;i++)
	{
		dmx_image_del(images[i]);
		free(sets[i]);
	}
	step=0;
}


static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
			


	step++;

	if((step%3) == 1)
	{
		unsigned int lamp=10;
		while(lamp > 3)
			lamp = rand()&0xf;

		if(sets[lamp]->dim>0.1f)
			return 100;
			

		switch(rand()%8)
		{
			case 0:
				dmx_image_set_selector(images[lamp],"LP COL","red");
				break;
			case 1:
				dmx_image_set_selector(images[lamp],"LP COL","blue");
				break;
			case 2:
				dmx_image_set_selector(images[lamp],"LP COL","green");
				break;
			case 3:
				dmx_image_set_selector(images[lamp],"LP COL","orange");
				break;
			case 4:
				dmx_image_set_selector(images[lamp],"LP COL","cyan");
				break;
			case 5:
				dmx_image_set_selector(images[lamp],"LP COL","magenta");
				break;
			case 6:
				dmx_image_set_selector(images[lamp],"LP COL","yellow");
				break;
			default:
				dmx_image_set_selector(images[lamp],"LP COL","rose");
				break;
		}

		sets[lamp]->dim=1.0f;
		return 500;
	}
	else
	{
		for(int i = 0;i < PAR56_COUNT;i++)
		{
			if(sets[i]->dim > 0.3f)sets[i]->dim -= 0.3f; else sets[i]->dim=0.0f;
		}
		return 500;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

#if LUA_CUE==1
	dmx_queue_add("LED-SCENE-004",init,deinit,tick);
#else
	dmx_queue_activate(dmx_queue_add("LED-SCENE-004",init,deinit,tick));
#endif
}
