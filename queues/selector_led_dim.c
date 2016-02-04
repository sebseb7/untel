#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_selector.h"


static struct dmx_set* set1;

struct paramnames {
	const char* name;
	float value;
};

static struct paramnames paramlist[] = {

	{"on"    ,1.0f  },
	{"half"  ,0.5f  },
	{"off"   ,0.0f  },

};


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
	for(unsigned int i=0;i< (sizeof(paramlist)/sizeof(paramlist[0]));i++)
	{
		if(strncmp(name,paramlist[i].name,DMX_NAME_LENGTH)==0)
		{
			return i+1;
		}
	}
	return 0;
}

static void position(unsigned int index)
{
	if(index > 	(sizeof(paramlist)/sizeof(paramlist[0])))
		return;

	set1->dim=paramlist[index-1].value;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {
	
	struct dmx_selector* selector = dmx_selector_add("LP DIM",init,deinit,getname,getidbyname,position,(sizeof(paramlist)/sizeof(paramlist[0])));

	
	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_DIM;
	set1->dim=0.0f;
	dmx_selector_attach_set(selector,set1);

}


