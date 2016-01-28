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
	void (*init)(void);
	void (*deinit)(void);
	
	unsigned int dev_alloc;
	unsigned int dev_count;
	char** dev_names;
	unsigned int* dev_types;	
	
};

struct dmx_selector * dmx_selector_add(char* name,void(*init)(void),void(*deinit)(void),void(*name)(unsigned int,char*),void(*render)(unsigned int),unsigned int length);
void dmx_selector_add_device(struct dmx_selector* selector,unsigned int type, char* name);
void dmx_selector_set(struct dmx_selector* selector,unsigned int pos);

*/


#define DMX_SELECTOR_ALLOCATE_INITIAL 100
#define DMX_SELECTOR_ALLOCATE_STEP 10

#define DMX_SELECTOR_DEVLIST_ALLOCATE_INITIAL 10
#define DMX_SELECTOR_SETLIST_ALLOCATE_INITIAL 10

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

struct dmx_selector * dmx_selector_add(char* name,void(*init)(void),void(*deinit)(void),void(*getname)(unsigned int,char*),void(*render)(unsigned int),unsigned int length)
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
	selector->init=init;
	selector->deinit=deinit;
	selector->getname=getname;
	selector->render=render;
	selector->length=length;
	selector->active=0;

	selector->dev_alloc=DMX_SELECTOR_DEVLIST_ALLOCATE_INITIAL;
	selector->dev_count=0;
	selector->dev_names=malloc(sizeof(char)*DMX_NAME_LENGTH*DMX_SELECTOR_DEVLIST_ALLOCATE_INITIAL);
	selector->dev_types=malloc(sizeof(unsigned int)*DMX_SELECTOR_DEVLIST_ALLOCATE_INITIAL);
	
	selector->sets_alloc=DMX_SELECTOR_SETLIST_ALLOCATE_INITIAL;
	selector->set_count=0;
	selector->set_list=malloc(sizeof(struct dmx_set*)*DMX_SELECTOR_SETLIST_ALLOCATE_INITIAL);

	return selector;
}

void dmx_selector_set(struct dmx_selector* selector,unsigned int pos)
{
	if(pos == 0)
	{
		if(selector->active != 0)
			selector->deinit();
	}
	else if(pos <= selector->length)
	{
		if(selector->active == 0)
		{
			selector->init();
		}
		selector->render(pos);
	}
	selector->active=pos;
}

void dmx_selector_add_device(struct dmx_selector* selector,unsigned int type, char* name)
{
	unsigned int index = selector->dev_count;

	if(selector->dev_count < selector->dev_alloc)
	{
		strncpy((char*)&(selector->dev_names[DMX_NAME_LENGTH*index]),name,DMX_NAME_LENGTH);
		selector->dev_types[index]=type;

		selector->dev_count++;
	}

}

void dmx_selector_attach_set(struct dmx_selector* selector,struct dmx_set* set)
{
	unsigned int index = selector->set_count;

	if(selector->set_count < selector->sets_alloc)
	{
		selector->set_list[index]=set;

		selector->set_count++;
	}

}

