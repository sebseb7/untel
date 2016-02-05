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
		
}

static void deinit(void)
{
	if(image2 != NULL)
		dmx_image_del(image2);
}

static unsigned int step = 0;

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			image2 = dmx_image_new(1);
			dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn links");
			dmx_image_set_selector(image2,"LP COL","red");
			dmx_image_set_selector(image2,"LP DIM","on");
			dmx_image_show(image2);
			return 100;
		default:
			dmx_image_del(image2);
			image2 = NULL; 
			step=0;
			return 300;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-WHITEFLASH",init,deinit,tick);

}


