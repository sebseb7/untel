#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"


static struct dmx_image* image1;
static struct dmx_set* set1;
static struct dmx_set* set2;

static void init(void)
{
	dmx_device_create_strobe(1,"strobe-1");
		
	image1 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_STROBE,"strobe-1");
	set1 = dmx_set_new_dim(1.0f);
	set2 = dmx_set_new_freq(0.5f);
	dmx_image_attach_set(image1,set1);
	dmx_image_attach_set(image1,set2);
	dmx_image_show(image1);

}

static unsigned int step = 0;
static void deinit(void)
{
	dmx_image_del(image1);
	free(set1);
	free(set2);
	step=0;
}


static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{

	step++;

	return 500;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("STROBE-SCENE-001",init,deinit,tick);

}
