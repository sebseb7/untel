#include <string.h>
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

static unsigned int getidbyname(char* name)
{
	if(strncmp(name,"red",DMX_NAME_LENGTH)==0)
	{
		return 1;
	}
	else if(strncmp(name,"blue",DMX_NAME_LENGTH)==0)
	{
		return 2;
	}
	else if(strncmp(name,"green",DMX_NAME_LENGTH)==0)
	{
		return 3;
	}
	return 0;
}


static void position(__attribute__((__unused__)) unsigned int index)
{
	unsigned int red=0;
	unsigned int green=0;
	unsigned int blue=0;

	switch(index)
	{
		case 1:
			red=255;
			break;
		case 2:
			blue=255;
			break;
		case 3:
			green=255;
			break;
	
	}
		
	set1->color[0]=red;
	set1->color[1]=green;
	set1->color[2]=blue;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {
	
	struct dmx_selector* selector = dmx_selector_add("LP COL",init,deinit,getname,getidbyname,position,1);

	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_COLSET;
	set1->color[0]=0;
	set1->color[1]=0;
	set1->color[2]=0;
	dmx_selector_attach_set(selector,set1);

}


