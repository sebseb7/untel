#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"
#include "par56.h"


static struct dmx_image* images[4];
static struct dmx_set* sets[4];
static unsigned int activated = 0;

static void init(void)
{
	create_par56();
		
	images[0] = dmx_image_new(0);
	images[1] = dmx_image_new(0);
	images[2] = dmx_image_new(0);
	images[3] = dmx_image_new(0);
	dmx_image_add_device(images[0],DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(images[1],DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(images[2],DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(images[3],DMX_DEVICE_LEDPAR6,"par-4");
	sets[0] = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(images[0],sets[0]);
	sets[1] = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(images[1],sets[1]);
	sets[2] = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(images[2],sets[2]);
	sets[3] = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(images[3],sets[3]);
	dmx_image_show(images[0]);
	dmx_image_show(images[1]);
	dmx_image_show(images[2]);
	dmx_image_show(images[3]);
	
	activated = dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDE"));

}

static unsigned int step = 0;
static void deinit(void)
{
	if(activated) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SLIDE"));
	activated=0;
	dmx_image_del(images[0]);
	free(sets[0]);
	dmx_image_del(images[1]);
	free(sets[1]);
	dmx_image_del(images[2]);
	free(sets[2]);
	dmx_image_del(images[3]);
	free(sets[3]);
	step=0;
}

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	
	if(activated ==0) activated=dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDE"));


	step++;

	if(step == 1)
	{
		unsigned int lamp = rand() % 4;


		sets[lamp]->dim=1.0f;
		return 500;
	}
	else
	{
		sets[0]->dim=0.0f;
		sets[1]->dim=0.0f;
		sets[2]->dim=0.0f;
		sets[3]->dim=0.0f;
		step=0;
		return 800;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-003",init,deinit,tick);

}
