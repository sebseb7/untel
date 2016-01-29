#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image1;

static void init(void)
{
	dmx_device_create_ledpar(8,0,"vorn links");
	dmx_device_create_ledpar(16,0,"vorn rechts");
	dmx_device_create_ledpar(24,0,"hinten links");
	dmx_device_create_ledpar(32,0,"hinten rechts");
		
	image1 = dmx_image_new();
	
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_add_selector(image1,"LP DIM","on");
	dmx_image_add_selector(image1,"LP COL","red");

	printf("image1 initialized\n");
}

static void deinit(void)
{
}


static void tick(__attribute__((__unused__)) unsigned int time)
{
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add(init,deinit,tick);

}


