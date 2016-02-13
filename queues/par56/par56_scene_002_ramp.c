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

	if(0==(rand() % 10))dmx_image_set_selector(image1,"LP COL","blue");
	if(0==(rand() % 25))dmx_image_set_selector(image1,"LP COL","white");
	if(0==(rand() % 50))dmx_image_set_selector(image1,"LP COL","green");

	step++;

	if(step < 30)
	{
		if((step&1) == 0)
			set1->dim = 0.0f;
		if((step&1) == 1)
			set1->dim = (step/30.0f)*1.0f;
		return 500;
	}
	else
	{
		set1->dim=0.0f;
		step=0;
		return 40000;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-002",init,deinit,tick);

}
