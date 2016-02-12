#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image2;
static unsigned int step = 0;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image2 = dmx_image_new(0);
	
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn rechts");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"hinten links");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"hinten rechts");
	dmx_image_set_selector(image2,"LP COL","green");

	dmx_image_show(image2);
}

static void deinit(void)
{
	dmx_image_del(image2);
	step=0;
}


static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			dmx_image_blend_selector(image2,"LP COL","red",20000);
			return 20000;
		case 1:
			dmx_image_blend_selector(image2,"LP COL","blue",20000);
			return 20000;
		case 2:
			dmx_image_blend_selector(image2,"LP COL","green",20000);
			return 20000;
		case 3:
			dmx_image_blend_selector(image2,"LP COL","orange",20000);
			return 20000;
		case 4:
			dmx_image_blend_selector(image2,"LP COL","cyan",20000);
			return 20000;
		case 5:
			dmx_image_blend_selector(image2,"LP COL","magenta",20000);
			return 20000;
		case 6:
			dmx_image_blend_selector(image2,"LP COL","yellow",20000);
			return 20000;
		default:
			dmx_image_blend_selector(image2,"LP COL","rose",20000);
			step=0;
			return 20000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-COL-SLIDE",init,deinit,tick);

}


