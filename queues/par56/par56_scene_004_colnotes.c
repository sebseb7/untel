#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_globvar.h"


static struct dmx_image* images[6];
static struct dmx_set* sets[6];
static struct dmx_image* image1;
static struct dmx_image* image2;
static struct dmx_image* image3;
static struct dmx_image* image4;
static struct dmx_image* image5;
static struct dmx_image* image6;
static struct dmx_set* set1;
static struct dmx_set* set2;
static struct dmx_set* set3;
static struct dmx_set* set4;
static struct dmx_set* set5;
static struct dmx_set* set6;

static void init(void)
{
	dmx_device_create_ledpar6(8,"par-1");
	dmx_device_create_ledpar6(16,"par-2");
	dmx_device_create_ledpar6(24,"par-3");
	dmx_device_create_ledpar6(32,"par-4");
	dmx_device_create_ledpar6(40,"par-5");
	dmx_device_create_ledpar6(48,"par-6");
		
	image1 = dmx_image_new(0);
	image2 = dmx_image_new(0);
	image3 = dmx_image_new(0);
	image4 = dmx_image_new(0);
	image5 = dmx_image_new(0);
	image6 = dmx_image_new(0);
	dmx_image_add_device(image1,DMX_DEVICE_LEDPAR6,"par-1");
	dmx_image_add_device(image2,DMX_DEVICE_LEDPAR6,"par-2");
	dmx_image_add_device(image3,DMX_DEVICE_LEDPAR6,"par-3");
	dmx_image_add_device(image4,DMX_DEVICE_LEDPAR6,"par-4");
	dmx_image_add_device(image5,DMX_DEVICE_LEDPAR6,"par-5");
	dmx_image_add_device(image6,DMX_DEVICE_LEDPAR6,"par-6");
	set1 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image1,set1);
	set2 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image2,set2);
	set3 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image3,set3);
	set4 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image4,set4);
	set5 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image5,set5);
	set6 = dmx_set_new_dim(0.0f);
	dmx_image_attach_set(image6,set6);
	dmx_image_show(image1);
	dmx_image_show(image2);
	dmx_image_show(image3);
	dmx_image_show(image4);
	dmx_image_show(image5);
	dmx_image_show(image6);


	sets[0]=set1;
	sets[1]=set2;
	sets[2]=set3;
	sets[3]=set4;
	sets[4]=set5;
	sets[5]=set6;
	images[0]=image1;
	images[1]=image2;
	images[2]=image3;
	images[3]=image4;
	images[4]=image5;
	images[5]=image6;
	

}

static unsigned int step = 0;
static void deinit(void)
{
	dmx_image_del(image1);
	free(set1);
	dmx_image_del(image2);
	free(set2);
	dmx_image_del(image3);
	free(set3);
	dmx_image_del(image4);
	free(set4);
	dmx_image_del(image5);
	free(set5);
	dmx_image_del(image6);
	free(set6);
	step=0;
}


static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
			


	step++;

	if((step%8) == 1)
	{
		unsigned int lamp=10;
		while(lamp > 5)
			lamp = rand()&0xf;

		switch(rand()%7)
		{
			case 0:
				dmx_image_set_selector(images[lamp],"LP COL","red");
				break;
			case 1:
				dmx_image_set_selector(images[lamp],"LP COL","blue");
				break;
			case 2:
				dmx_image_set_selector(images[lamp],"LP COL","green");
				break;
			case 3:
				dmx_image_set_selector(images[lamp],"LP COL","orange");
				break;
			case 4:
				dmx_image_set_selector(images[lamp],"LP COL","cyan");
				break;
			case 5:
				dmx_image_set_selector(images[lamp],"LP COL","magenta");
				break;
			case 6:
				dmx_image_set_selector(images[lamp],"LP COL","yellow");
				break;
			default:
				dmx_image_set_selector(images[lamp],"LP COL","rose");
				break;
		}



		sets[lamp]->dim=1.0f;
		return 500;
	}
	else
	{
		if(set1->dim > 0.01f)set1->dim /= 2.0f;
		if(set2->dim > 0.01f)set2->dim /= 2.0f;
		if(set3->dim > 0.01f)set3->dim /= 2.0f;
		if(set4->dim > 0.01f)set4->dim /= 2.0f;
		if(set5->dim > 0.01f)set5->dim /= 2.0f;
		if(set6->dim > 0.01f)set6->dim /= 2.0f;
		return 500;
	}
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-SCENE-004",init,deinit,tick);

}
