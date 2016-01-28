#include <stdlib.h>

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
	
	image1 = dmx_image_add(DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_add_setDim(image1,0.0f);
}

static void deinit(void)
{
}



static void tick(__attribute__((__unused__)) unsigned int time)
{
	static unsigned int step =0;	

	dmx_image_del(image1);

	image1 = dmx_image_add(DMX_DEVICE_LEDPAR,"vorn links");
	dmx_image_add_setDim(image1,step/20.0f);


	step++;
	if(step==20)
		step=0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add(init,deinit,tick);

}


