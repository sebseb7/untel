#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static void init(void)
{
	dmx_device_create_ledpar(8,0,"vorn links");
	dmx_device_create_ledpar(16,0,"vorn rechts");
	dmx_device_create_ledpar(24,0,"hinten links");
	dmx_device_create_ledpar(32,0,"hinten rechts");
}

static void deinit(void)
{
}


struct dmx_image* image1;

static void tick(__attribute__((__unused__)) unsigned int time)
{
	static unsigned int step =0;	

	if(step == 0)
	{
		image1 = dmx_image_add(DMX_DEVICE_LEDPAR,"vorn links");
		dmx_image_add_setCol(image1,255,0,0);
		dmx_image_add_setDim(image1,0.5f);
	}
	else if(step == 1)
	{
		dmx_image_del(image1);
	}


	step++;
	if(step==2)
		step=0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {

	dmx_queue_add(init,deinit,tick);

}


