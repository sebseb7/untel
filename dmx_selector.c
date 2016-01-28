#include <stdlib.h>
#include <string.h>

#include "dmx_selector.h"


/*
struct dmx_selector
{
	char name[DMX_NAME_LENGTH];
	unsigned int length;
	unsigned int active;
	void (*render)(unsigned int);
	void (*getname)(unsigned int,char*);
	void (*deinit)(void);
	
	unsigned int dev_alloc;
	unsigned int dev_count;
	struct dmx_device* dev_list;
	
};

struct dmx_selector * dmx_selector_add(char* name,void(*deinit)(void),void(*name)(unsigned int,char*),void(*render)(unsigned int),unsigned int length);
void dmx_selector_add_device(struct dmx_selector* selector,unsigned int type, char* name);
void dmx_selector_set(struct dmx_selector* selector,unsigned int pos);

*/


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

struct dmx_selector * dmx_selector_add(char* name,void(*deinit)(void),void(*getname)(unsigned int,char*),void(*render)(unsigned int),unsigned int length)
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

	strncpy(selector->name,name,DMX_NAME_LENGTH);
	selector->deinit=deinit;
	selector->getname=getname;
	selector->render=render;
	selector->length=length;
	selector->active=0;
	return selector;
}

void dmx_selector_set(struct dmx_selector* selector,unsigned int pos)
{
	selector->active=pos;
	
	if(pos == 0)
	{
		selector->deinit;
	}
	else if(pos <= selector->length)
	{
		selector->render(pos);
	}
}

void dmx_selector_add_device(struct dmx_selector* selector,unsigned int type, char* name)
{
}

