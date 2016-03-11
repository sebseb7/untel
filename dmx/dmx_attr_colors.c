#include <stdlib.h>

#include "dmx_attr_colors.h"

struct colornames {
	const char* name;
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
static unsigned int colornameslistcount =  9;


unsigned int dmx_attr_colors_get_count(void)
{
	return colornameslistcount;
}
const char* dmx_attr_colors_get_name(unsigned int idx)
{
	if(idx < colornameslistcount)
	{
		return colornameslist[idx].name;
	}
	return NULL;
}


