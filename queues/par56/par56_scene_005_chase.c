#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"
#include "par56.h"


static struct dmx_image* images[PAR56_COUNT];
static struct dmx_set* sets[PAR56_COUNT];
static unsigned int activated = 0;
static unsigned int lamp = 0;

static void init(void)
{
	create_par56();

	for(int i = 0;i < PAR56_COUNT;i++) images[i] = dmx_image_new(0);
	dmx_image_add_device(images[0],DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(images[1],DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(images[2],DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(images[3],DMX_DEVICE_LEDPAR6,"par-4");
	for(int i = 0;i < PAR56_COUNT;i++)
	{
		sets[i] = dmx_set_new_dim(0.0f);
		dmx_image_attach_set(images[i],sets[i]);
		dmx_image_show(images[i]);
	}

	activated = dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDEfast"));

}

static unsigned int step = 0;
static void deinit(void)
{
	if(activated) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SLIDEfast"));
	activated=0;
	for(int i = 0;i < PAR56_COUNT;i++)
	{
		dmx_image_del(images[i]);
		free(sets[i]);
	}
	step=0;
}

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{

	if(activated ==0) activated=dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDEfast"));

	for(int i = 0;i < PAR56_COUNT;i++)
	{
		if(sets[i]->dim > 0.0f)
			sets[i]->dim-=0.3f;
		if(sets[i]->dim < 0.0f)
			sets[i]->dim=0.0f;
	}


	step++;

	if(step == 1)
	{
		lamp++;
		if(lamp==6) lamp=0;

		if(lamp < 4) sets[lamp]->dim=1.0f;
	}
	if(step==2) step=0;
	return 750;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-005",init,deinit,tick);

}
