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
   char* name;
   unsigned int active_cnt;
   unsigned int button;
   unsigned int type; //0=loop;1=1loop
   struct dmx_luastep* start;
   struct dmx_luastep* last;
   struct dmx_luaqueue* next;
   struct dmx_luastep* playing_next;
   unsigned int play_at;
//locks hold (to prevent queue or select locks at queue end
};
*/

static struct dmx_luaqueue* dmx_luaqueue_start = NULL;
static struct dmx_luaqueue* dmx_luaqueue_last = NULL;
static unsigned int dmx_luaqueue_count=0;

struct dmx_luaqueue* dmx_luaqueue_add(const char* name)
{
	struct dmx_luaqueue* luaqueue = malloc(sizeof(struct dmx_luaqueue));

	luaqueue->name = strndup(name,DMX_NAME_LENGTH);
	luaqueue->active_cnt=0;
	luaqueue->button=0;
	luaqueue->type=0;
	luaqueue->start_step=NULL;
	luaqueue->last_step=NULL;
	luaqueue->next=NULL;
	luaqueue->playing_next=NULL;
	luaqueue->play_at=0;


	if(dmx_luaqueue_start == NULL)
	{
		dmx_luaqueue_start = luaqueue;
	}
	if(dmx_luaqueue_last != NULL)
	{
		dmx_luaqueue_last->next=luaqueue;
	}
	dmx_luaqueue_last=luaqueue;


	dmx_luaqueue_count++;

	return luaqueue;
}

struct dmx_luastep* dmx_luaqueue_addstep(struct dmx_luaqueue* luaqueue,const char* code,unsigned int hold,unsigned int hold_unit,unsigned int blend,unsigned int blend_unit)
{
	struct dmx_luastep* luastep = malloc(sizeof(struct dmx_luastep));

	luastep->code = strdup(code);
	luastep->hold = hold;
	luastep->blend = blend;
	luastep->hold_unit = hold_unit;
	luastep->blend_unit = blend_unit;
	luastep->next = NULL;	

	if(luaqueue->start_step == NULL)
	{
		luaqueue->start_step = luastep;
	}
	if(luaqueue->last_step != NULL)
	{
		(luaqueue->last_step)->next=luastep;
	}
	luaqueue->last_step=luastep;


	return luastep;

}

void dmx_luaqueue_activate(struct dmx_luaqueue* luaqueue)
{
	if(luaqueue->active_cnt==0)
	{
		luaqueue->playing_next=luaqueue->start_step;
		luaqueue->next = 0;
	}

	luaqueue->active_cnt++;
}
void dmx_luaqueue_cleanup_locks(struct dmx_luaqueue* luaqueue)
{
	luaqueue->active_cnt = luaqueue->active_cnt;
}

void dmx_luaqueue_deactivate(struct dmx_luaqueue* luaqueue)
{
	if(luaqueue->active_cnt==1)
		dmx_luaqueue_cleanup_locks(luaqueue);

	if(luaqueue->active_cnt!=0)
		luaqueue->active_cnt--;
}


#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int lua_test (lua_State *L) {
	double d = lua_tonumber(L, 1);  /* get argument */
	printf("l_t %f\n",d);
	d=d*2;
	lua_pushnumber(L, d);  /* push result */
	return 1;  /* number of results */
}

static void dmx_luaqueue_process_step(struct dmx_luaqueue* luaqueue,struct dmx_luastep* step)
{
	//execute lua
	lua_State* L;
	L=luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, lua_test);
	lua_setglobal(L, "l_t");
	if(luaL_loadstring(L,"a=0;for i=1,10 do;a=a+1;end;a=l_t(a);return a") == 0)
	{
		lua_pcall(L, 0, LUA_MULTRET, 0);
	}
	printf("%f\n",lua_tonumber(L,-1));
	lua_close(L); 
}


static void dmx_luaqueue_process_next(struct dmx_luaqueue* luaqueue,unsigned int time,unsigned int bpm)
{
	if(
			(luaqueue->active_cnt != 0)&&
			(luaqueue->play_at <= time)&&
			(luaqueue->playing_next != NULL)
	  )
	{
		dmx_luaqueue_process_step(luaqueue,luaqueue->playing_next);
		//set next step
		//set time for next execution
	}


	if(luaqueue->next != NULL)
	{
		dmx_luaqueue_process_next(luaqueue->next,time,bpm);
	}

}

void dmx_luaqueue_process_all(unsigned int time,unsigned int bpm)
{
	if(dmx_luaqueue_start == NULL) return;
	dmx_luaqueue_process_next(dmx_luaqueue_start,time,bpm);
}
