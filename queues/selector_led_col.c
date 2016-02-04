#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_selector.h"


static struct dmx_set* set1;

struct colornames {
	const char* color;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

static struct colornames colornameslist[] = {

	{"red"    ,255,0  ,0  },
	{"green"  ,0  ,255,0  },
	{"blue"   ,0  ,0  ,255},
	{"yellow" ,255,100,0  },
	{"magenta",255,0  ,203},
	{"orange" ,255,15 ,0  },
	{"rose"   ,255,23 ,23 },
	{"cyan"   ,0  ,255,255},
	{"white"  ,255,255,255}

};

static unsigned int getidbyname(char* name)
{
	for(unsigned int i=0;i< (sizeof(colornameslist)/sizeof(colornameslist[0]));i++)
	{
		if(strncmp(name,colornameslist[i].color,DMX_NAME_LENGTH)==0)
		{
			return i+1;
		}
	}
	return 0;
}

static void position(unsigned int index)
{
	if(index > 	(sizeof(colornameslist)/sizeof(colornameslist[0])))
		return;

	set1->color[0]=colornameslist[index-1].red;
	set1->color[1]=colornameslist[index-1].green;
	set1->color[2]=colornameslist[index-1].blue;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_selector* selector = dmx_selector_add("LP COL",getidbyname,position,(sizeof(colornameslist)/sizeof(colornameslist[0])));

	set1=malloc(sizeof(struct dmx_set));
	set1->attr_type=ATTR_TYPE_COLSET;
	set1->color[0]=0;
	set1->color[1]=0;
	set1->color[2]=0;
	dmx_selector_attach_set(selector,set1);
}


