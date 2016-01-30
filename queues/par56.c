#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image1;
static struct dmx_image* image2;

static void init(void)
{
	dmx_device_create_ledpar(8,0,"vorn links");
	dmx_device_create_ledpar(16,0,"vorn rechts");
	dmx_device_create_ledpar(24,0,"hinten links");
	dmx_device_create_ledpar(32,0,"hinten rechts");
		
	image1 = dmx_image_new();
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_set_selector(image1,"LP DIM","on");
	dmx_image_show(image1);

	
	image2 = dmx_image_new();
	
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_set_selector(image2,"LP COL","red");

	dmx_image_show(image2);
}

static void deinit(void)
{
}


static unsigned int step = 0;

unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	step++;

	if(step == 1)
	{
		dmx_image_blend_selector(image2,"LP COL","red",4);
	}
	if(step == 2)
	{
		dmx_image_blend_selector(image2,"LP COL","blue",10);
	}



	if(step==2)
		step=0;

	return 24;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add(init,deinit,tick);

}


