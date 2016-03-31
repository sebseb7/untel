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

	{"fastest"    ,1.0f  },
	{"fast"  ,0.8f  },
	{"normal"   ,0.6f  },
	{"slow"   ,0.5f  },

};

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

	set1->freq=paramlist[index-1].value;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {
	
	struct dmx_selector* selector = dmx_selector_add("FREQ",getidbyname,position,(sizeof(paramlist)/sizeof(paramlist[0])));
	
	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_FREQ;
	set1->freq=0.0f;
	dmx_selector_attach_set(selector,set1);
}


