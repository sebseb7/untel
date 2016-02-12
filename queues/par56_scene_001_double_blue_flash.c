#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"

static struct dmx_image* image1;
static struct dmx_image* image2;
static struct dmx_image* image3;
static struct dmx_image* image4;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image1 = dmx_image_new(0);
	image2 = dmx_image_new(0);
	image3 = dmx_image_new(0);
	image4 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn rechts");
	dmx_image_add_device(image3,DMX_DEVICE_LEDPAR6,"hinten links");
	dmx_image_add_device(image4,DMX_DEVICE_LEDPAR6,"hinten rechts");
	dmx_image_set_selector(image1,"LP COL","blue");
	dmx_image_set_selector(image2,"LP COL","blue");
	dmx_image_set_selector(image3,"LP COL","blue");
	dmx_image_set_selector(image4,"LP COL","blue");
	dmx_image_show(image1);
	dmx_image_show(image2);
	dmx_image_show(image3);
	dmx_image_show(image4);
}

static unsigned int step = 0;
static unsigned int map = (1<<0)|(1<<1)|(1<<2)|(1<<3);
static void deinit(void)
{
	dmx_image_del(image1);
	dmx_image_del(image2);
	dmx_image_del(image3);
	dmx_image_del(image4);
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
			unsigned int substep = bar % 4;

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


			if(map & (1<<0))dmx_image_set_selector(image1,"LP DIM","on");
			if(map & (1<<1))dmx_image_set_selector(image2,"LP DIM","on");
			if(map & (1<<2))dmx_image_set_selector(image3,"LP DIM","on");
			if(map & (1<<3))dmx_image_set_selector(image4,"LP DIM","on");
			return 300;
		case 1:
			if(map & (1<<0))dmx_image_set_selector(image1,"LP DIM","off");
			if(map & (1<<1))dmx_image_set_selector(image2,"LP DIM","off");
			if(map & (1<<2))dmx_image_set_selector(image3,"LP DIM","off");
			if(map & (1<<3))dmx_image_set_selector(image4,"LP DIM","off");
			return 300;
		case 2:
			if(map & (1<<0))dmx_image_set_selector(image1,"LP DIM","on");
			if(map & (1<<2))dmx_image_set_selector(image2,"LP DIM","on");
			if(map & (1<<2))dmx_image_set_selector(image3,"LP DIM","on");
			if(map & (1<<3))dmx_image_set_selector(image4,"LP DIM","on");
			return 300;
		default:
			if(map & (1<<0))dmx_image_set_selector(image1,"LP DIM","off");
			if(map & (1<<1))dmx_image_set_selector(image2,"LP DIM","off");
			if(map & (1<<2))dmx_image_set_selector(image3,"LP DIM","off");
			if(map & (1<<3))dmx_image_set_selector(image4,"LP DIM","off");
			step=0;
			return 5000;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-001",init,deinit,tick);

}
