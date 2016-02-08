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
	struct dmx_luastep* start_step;
	struct dmx_luastep* last_step;
	struct dmx_luaqueue* next;
	struct dmx_luastep* playing_next;
	unsigned int play_at;
	//locks hold (to prevent queue or select locks at queue end
};

struct dmx_luaqueue* dmx_luaqueue_add(const char* name);
struct dmx_luastep* dmx_luaqueue_addstep(struct dmx_luaqueue* luaqueue,const char* code,unsigned int hold,unsigned int hold_unit,unsigned int blend,unsigned int blend_unit);

void dmx_luaqueue_process_all(unsigned int time,unsigned int bpm);
void dmx_luaqueue_deactivate(struct dmx_luaqueue* luaqueue);
void dmx_luaqueue_activate(struct dmx_luaqueue* luaqueue);

#endif

