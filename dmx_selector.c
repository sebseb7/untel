#include <stdlib.h>

#include "dmx_selector.h"




#define DMX_SELECTOR_ALLOCATE_INITIAL 100
#define DMX_SELECTOR_ALLOCATE_STEP 10

static struct dmx_selector** dmx_selector_list;

static unsigned int selector_allocated=0;
static unsigned int selector_inuse=0;

unsigned int dmx_selector_get_count(void)
{
	return selector_inuse;
}

struct dmx_selector* dmx_selector_getbyidx(unsigned int index)
{
	if(index < selector_inuse)
	{
		return dmx_selector_list[index];
	}
	return NULL;
}

struct dmx_selector * dmx_selector_add(void(*init)(void),void(*deinit)(void),void(*name)(unsigned int,char*),void(*render)(unsigned int))
{
	if(0==selector_allocated)
	{
		dmx_selector_list = malloc(sizeof(*dmx_selector_list)*DMX_SELECTOR_ALLOCATE_INITIAL);
		selector_allocated=DMX_SELECTOR_ALLOCATE_INITIAL;
	}

	if(selector_inuse == selector_allocated)
	{
		//implement realloc
		exit(EXIT_FAILURE);
	}
	struct dmx_selector* selector = malloc(sizeof(*selector));
	dmx_selector_list[selector_inuse]=selector;
	selector_inuse++;

	return selector;
}



