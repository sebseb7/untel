#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_luaqueue.h"
/*
struct dmx_luastep
{
	char* code;
	unsigned int hold;
	unsigned int hold_unit;
	unsigned int blend;
	unsigned int blend_unit;

	struct dmx_luastep* next;
};

struct dmx_luaqueue 
{
	char name[DMX_NAME_LENGTH];
	unsigned int active_cnt;
	unsigned int button;
	unsigned int type; //0=loop;1=1loop
	struct dmx_luastep* start;
	struct dmx_luaqueue* next;
	//locks hold (to prevent queue or select locks at queue end
};
*/

static struct dmx_luaqueue* dmx_luaqueue_start = NULL;
static struct dmx_luaqueue* dmx_luaqueue_last = NULL;


struct dmx_luaqueue* dmx_luaqueue_add(const char* name)
{
	struct dmx_luaqueue* luaqueue = malloc(sizeof(struct dmx_luaqueue));

	luaqueue->name = strndup(name,DMX_NAME_LENGTH);
	luaqueue->active_cnt=0;
	luaqueue->button=0;
	luaqueue->type=0;
	luaqueue->start=NULL;
	luaqueue->next=NULL;


	if(dmx_luaqueue_start == NULL)
	{
		dmx_luaqueue_start = luaqueue;
	}
	if(dmx_luaqueue_last != NULL)
	{
		dmx_luaqueue_last->next=luaqueue;
	}
	dmx_luaqueue_last=luaqueue;


	return luaqueue;
}

