#ifndef _DMX_LUAQUEUE_H
#define _DMX_LUAQUEUE_H

#include "dmx_devices.h"

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
	char* name;
	unsigned int active_cnt;
	unsigned int button;
	unsigned int type; //0=loop;1=1loop
	struct dmx_luastep* start;
	struct dmx_luaqueue* next;
	//locks hold (to prevent queue or select locks at queue end
};

struct dmx_luaqueue* dmx_luaqueue_add(const char* name);

#endif

