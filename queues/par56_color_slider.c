#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image2;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image2 = dmx_image_new();
	
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_set_selector(image2,"LP COL","green");

	dmx_image_show(image2);
}

static void deinit(void)
{
	dmx_image_del(image2);
}

static unsigned int step = 0;

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			dmx_image_set_selector(image2,"LP COL","red");
			return 20000;
		case 1:
			dmx_image_set_selector(image2,"LP COL","blue");
			return 20000;
		case 2:
			dmx_image_set_selector(image2,"LP COL","green");
			return 20000;
		case 3:
			dmx_image_set_selector(image2,"LP COL","orange");
			return 20000;
		case 4:
			dmx_image_set_selector(image2,"LP COL","cyan");
			return 20000;
		case 5:
			dmx_image_set_selector(image2,"LP COL","magenta");
			return 20000;
		case 6:
			dmx_image_set_selector(image2,"LP COL","yellow");
			return 20000;
		default:
			dmx_image_set_selector(image2,"LP COL","rose");
			step=0;
			return 20000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_queue* queue = dmx_queue_add("LED-COL-SLIDE",init,deinit,tick);
	dmx_queue_activate(queue);

}


