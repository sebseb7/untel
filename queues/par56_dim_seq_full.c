#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static struct dmx_image* image1;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image1 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_set_selector(image1,"LP DIM","on");
	dmx_image_show(image1);
}

static void deinit(void)
{
	dmx_image_del(image1);
}

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	return 200000;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-DIM-SEQ_ON",init,deinit,tick);

}