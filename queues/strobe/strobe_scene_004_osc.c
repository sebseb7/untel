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
	dmx_device_create_strobe(1,"strobe-1");
		
	image1 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_STROBE,"strobe-1");
	dmx_image_set_selector(image1,"LP DIM","low");
	set1 = dmx_set_new_freq(1.0f);
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
	if((step&1)==1)
	{
		dmx_image_blend_selector(image1,"LP DIM","on",10000);
	}
	else
	{
		dmx_image_blend_selector(image1,"LP DIM","off",10000);
	}

	return 10000;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("STROBE-SCENE-004",init,deinit,tick);

}
