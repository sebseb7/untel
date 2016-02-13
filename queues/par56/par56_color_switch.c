#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image2;
static unsigned int step = 0;

static void init(void)
{
	dmx_device_create_ledpar6(8,"par-1");
	dmx_device_create_ledpar6(16,"par-2");
	dmx_device_create_ledpar6(24,"par-3");
	dmx_device_create_ledpar6(32,"par-4");
	dmx_device_create_ledpar6(40,"par-5");
	dmx_device_create_ledpar6(48,"par-6");

	image2 = dmx_image_new(0);
	
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-4");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-5");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-6");

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
			dmx_image_set_selector(image2,"LP COL","cyan");
			return 3000;
		default:
			dmx_image_set_selector(image2,"LP COL","orange");
			step=0;
			return 3000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-COL-SWITCH",init,deinit,tick);

}


