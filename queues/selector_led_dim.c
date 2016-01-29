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
	if(strncmp(name,"on",DMX_NAME_LENGTH)==0)
	{
		return 1;
	}
	else if(strncmp(name,"half",DMX_NAME_LENGTH)==0)
	{
		return 2;
	}
	else if(strncmp(name,"of",DMX_NAME_LENGTH)==0)
	{
		return 3;
	}
	return 0;
}


static void position(__attribute__((__unused__)) unsigned int index)
{
	float dim=0.0f;


	switch(index)
	{
		case 1:
			dim=1.0f;
			break;
		case 2:
			dim=0.5f;
			break;
		case 3:
			dim=0.0f;
			break;
	
	}
		
		
	set1->dim=dim;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {
	
	struct dmx_selector* selector = dmx_selector_add("LP DIM",init,deinit,getname,getidbyname,position,1);

	
	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_DIM;
	set1->dim=0.0f;
	dmx_selector_attach_set(selector,set1);

}


