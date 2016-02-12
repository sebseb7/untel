#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"

static struct dmx_image* image1;
static struct dmx_set* set1;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image1 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"vorn rechts");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"hinten links");
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"hinten rechts");
	dmx_image_set_selector(image1,"LP COL","blue");

	set1 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image1,set1);
	

	dmx_image_show(image1);
}

static unsigned int step = 0;
static void deinit(void)
{
	dmx_image_del(image1);
	free(set1);
	step=0;
}

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{


	step++;

	if(step < 20)
	{
		if((step&1) == 0)
			set1->dim = 0.0f;
		if((step&1) == 1)
			set1->dim = (step/20.0f)*1.0f;
		return 400;
	}
	else
	{
		set1->dim=0.0f;
		step=0;
		return 20000;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-002",init,deinit,tick);

}
