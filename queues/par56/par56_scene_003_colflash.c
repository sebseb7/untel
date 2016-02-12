#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"


static struct dmx_set* sets[4];
static struct dmx_image* image1;
static struct dmx_image* image2;
static struct dmx_image* image3;
static struct dmx_image* image4;
static struct dmx_set* set1;
static struct dmx_set* set2;
static struct dmx_set* set3;
static struct dmx_set* set4;
static unsigned int activated = 0;

static void init(void)
{
	dmx_device_create_ledpar6(8,"vorn links");
	dmx_device_create_ledpar6(16,"vorn rechts");
	dmx_device_create_ledpar6(24,"hinten links");
	dmx_device_create_ledpar6(32,"hinten rechts");
		
	image1 = dmx_image_new(0);
	image2 = dmx_image_new(0);
	image3 = dmx_image_new(0);
	image4 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"vorn links");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"vorn rechts");
	dmx_image_add_device(image3,DMX_DEVICE_LEDPAR6,"hinten links");
	dmx_image_add_device(image4,DMX_DEVICE_LEDPAR6,"hinten rechts");
	set1 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image1,set1);
	set2 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image2,set2);
	set3 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image3,set3);
	set4 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image4,set4);
	dmx_image_show(image1);
	dmx_image_show(image2);
	dmx_image_show(image3);
	dmx_image_show(image4);


	sets[0]=set1;
	sets[1]=set2;
	sets[2]=set3;
	sets[3]=set4;
	
	activated = dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDE"));

}

static unsigned int step = 0;
static void deinit(void)
{
	if(activated) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SLIDE"));
	activated=0;
	dmx_image_del(image1);
	free(set1);
	dmx_image_del(image2);
	free(set2);
	dmx_image_del(image3);
	free(set3);
	dmx_image_del(image4);
	free(set4);
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
		set1->dim=0.0f;
		set2->dim=0.0f;
		set3->dim=0.0f;
		set4->dim=0.0f;
		step=0;
		return 800;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-003",init,deinit,tick);

}
