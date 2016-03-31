#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "par56.h"

static struct dmx_image* image2;
static unsigned int step = 0;

static void init(void)
{
	create_par56();
		
	image2 = dmx_image_new(0);
	
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-4");
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
			dmx_image_blend_selector(image2,"LP COL","red",5000);
			return 5000;
		case 1:
			dmx_image_blend_selector(image2,"LP COL","blue",5000);
			return 5000;
		case 2:
			dmx_image_blend_selector(image2,"LP COL","green",5000);
			return 5000;
		case 3:
			dmx_image_blend_selector(image2,"LP COL","orange",5000);
			return 5000;
		case 4:
			dmx_image_blend_selector(image2,"LP COL","cyan",5000);
			return 5000;
		case 5:
			dmx_image_blend_selector(image2,"LP COL","magenta",5000);
			return 5000;
		case 6:
			dmx_image_blend_selector(image2,"LP COL","yellow",5000);
			return 5000;
		default:
			dmx_image_blend_selector(image2,"LP COL","rose",5000);
			step=0;
			return 5000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-COL-SLIDEfast",init,deinit,tick);

}


