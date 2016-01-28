#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_selector.h"


static struct dmx_set* set1;



static void init(void)
{
}

static void deinit(void)
{
}

static void getname(__attribute__((__unused__)) unsigned int idx,__attribute__((__unused__)) char* name)
{
}


static void position(__attribute__((__unused__)) unsigned int index)
{
	unsigned int red=0;
	unsigned int green=0;
	unsigned int blue=0;


	if(index == 0)
	{
		red=255;
	}
		
		
	set1->color[0]=red;
	set1->color[1]=green;
	set1->color[2]=blue;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {
	
	dmx_device_create_ledpar(1,0,"bar");

	struct dmx_selector* selector = dmx_selector_add("LP COL",init,deinit,getname,position,1);

	dmx_selector_add_device(selector,DMX_DEVICE_LEDPAR,"bar");
	
	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_COLSET;
	set1->color[0]=0;
	set1->color[1]=0;
	set1->color[2]=0;
	dmx_selector_attach_set(selector,set1);



	dmx_selector_set(selector,1);

}


