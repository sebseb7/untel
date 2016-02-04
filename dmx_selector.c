#include <stdio.h>
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
struct dmx_selector* dmx_selector_getbyname(char* name)
{	
	for(unsigned int i=0;i<selector_inuse;i++)
	{
		if(0==strncmp(dmx_selector_list[i]->name,name,DMX_NAME_LENGTH))
		{
			return dmx_selector_list[i];
		}
	}
	return NULL;
}
struct dmx_selector * dmx_selector_add(char* name,void(*init)(void),void(*deinit)(void),void(*getname)(unsigned int,char*),unsigned int(*getidbyname)(char *),void(*render)(unsigned int),unsigned int length)
{
	if(0==selector_allocated)
	{
		dmx_selector_list = malloc(sizeof(*dmx_selector_list)*DMX_SELECTOR_ALLOCATE_INITIAL);
		selector_allocated=DMX_SELECTOR_ALLOCATE_INITIAL;
	}

	if(selector_inuse == selector_allocated)
	{
		//implement realloc
		printf("f4\n");
		exit(EXIT_FAILURE);
	}
	struct dmx_selector* selector = malloc(sizeof(*selector));
	dmx_selector_list[selector_inuse]=selector;
	selector_inuse++;

	strncpy(selector->name,name,DMX_NAME_LENGTH);
	selector->init=init;
	selector->deinit=deinit;
	selector->getname=getname;
	selector->getidbyname=getidbyname;
	selector->render=render;
	selector->length=length;

	
	selector->sets_alloc=DMX_SELECTOR_SETLIST_ALLOCATE_INITIAL;
	selector->set_count=0;
	selector->set_list=malloc(sizeof(struct dmx_set*)*DMX_SELECTOR_SETLIST_ALLOCATE_INITIAL);

	return selector;
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

void dmx_selector_print(void)
{
	for(unsigned int i=0;i<selector_inuse;i++)
	{
		struct dmx_selector* selector = dmx_selector_list[i];
		printf("sel name:%s len:%i\n",selector->name,selector->length);

	}
	printf("\n");
}



