#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image1;
static unsigned int step = 0;

static void init(void)
{
	dmx_device_create_ledpar6(8,"par-1");
	dmx_device_create_ledpar6(16,"par-2");
	dmx_device_create_ledpar6(24,"par-3");
	dmx_device_create_ledpar6(32,"par-4");
	dmx_device_create_ledpar6(40,"par-5");
	dmx_device_create_ledpar6(48,"par-6");
		
	image1 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-4");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-5");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-6");
	dmx_image_set_selector(image1,"LP DIM","on");
	dmx_image_show(image1);

	
}

static void deinit(void)
{
	dmx_image_del(image1);
	step=0;
}



#define beatsmul 1
static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			dmx_image_blend_selector(image1,"LP DIM","low",20000);
			return 20000;
		default:
			dmx_image_blend_selector(image1,"LP DIM","on",20000);
			step=0;
			return 20000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-DIM-SEQ2",init,deinit,tick);

}


