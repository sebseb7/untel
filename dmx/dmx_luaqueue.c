#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_luaqueue.h"
#include "dmx_queue.h"
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
	luaqueue->firstlock=NULL;
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

static void dmx_luaqueue_addlock(struct dmx_luaqueue* luaqueue,const char* name)
{
	struct dmx_luaqueuelock* iter_lock = luaqueue->firstlock;
	struct dmx_luaqueuelock* last_lock;

	while(iter_lock != NULL)
	{
		if(strncmp(iter_lock->name,name,DMX_NAME_LENGTH)==0)
		{
			return;
		}
		last_lock = iter_lock;
		iter_lock = iter_lock->next;
	}

	struct dmx_luaqueuelock* luaqueuelock = malloc(sizeof(struct dmx_luaqueuelock));
	luaqueuelock->name = strndup(name,DMX_NAME_LENGTH);
	luaqueuelock->next = NULL;

	if(luaqueue->firstlock == NULL)
	{
		luaqueue->firstlock = luaqueuelock;
	}
	else
	{
		last_lock->next=luaqueuelock;
	}

	struct dmx_queue* queue = dmx_queue_getbyname(name);
	if(queue != NULL)
	{
		dmx_queue_activate(queue);
	}
}

static void dmx_luaqueue_dellock(struct dmx_luaqueue* luaqueue,const char* name)
{
	struct dmx_luaqueuelock* iter_lock = luaqueue->firstlock;
	struct dmx_luaqueuelock* last_lock=NULL;

	while(iter_lock != NULL)
	{
		if(strncmp(iter_lock->name,name,DMX_NAME_LENGTH)==0)
		{
			if(last_lock==NULL)
			{
				luaqueue->firstlock=iter_lock->next;
			}
			else
			{
				last_lock->next = iter_lock->next;
			}
			free(iter_lock->name);
			free(iter_lock);

			//actually deactivating the queue

			struct dmx_queue* queue = dmx_queue_getbyname(name);
			if(queue != NULL)
			{
				dmx_queue_deactivate(queue);
			}

			return;
		}
		last_lock = iter_lock;
		iter_lock = iter_lock->next;
	}
}



#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static struct dmx_luaqueue* tmp_luaqueue_glob;
static struct dmx_luastep* tmp_luastep_glob;

static int dmx_luaqueue_callback_queue_on(lua_State *L) {
	const char* queuename = lua_tolstring(L, 1,NULL);
	dmx_luaqueue_addlock(tmp_luaqueue_glob,queuename);

	//printf("queue on %s\n",queuename);
	return 0; 
}
static int dmx_luaqueue_callback_queue_off(lua_State *L) {
	const char* queuename = lua_tolstring(L, 1,NULL);
	dmx_luaqueue_dellock(tmp_luaqueue_glob,queuename);
	//printf("queue off %s\n",queuename);
	return 0;
}

static void dmx_luaqueue_process_step(struct dmx_luaqueue* luaqueue,struct dmx_luastep* step)
{
	tmp_luaqueue_glob=luaqueue;
	tmp_luastep_glob=step;
	//execute lua
	lua_State* L;
	L=luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, dmx_luaqueue_callback_queue_on);
	lua_setglobal(L, "queue_on");
	lua_pushcfunction(L, dmx_luaqueue_callback_queue_off);
	lua_setglobal(L, "queue_off");
	if(luaL_loadstring(L,step->code) == 0)
	{
		lua_pcall(L, 0, LUA_MULTRET, 0);
	}
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
		
		if(luaqueue->playing_next->hold_unit == 0)
		{
			luaqueue->play_at = time+(luaqueue->playing_next->hold);
		}
		else
		{
			luaqueue->play_at = time+(luaqueue->playing_next->hold);
		}
		
		if(luaqueue->playing_next == luaqueue->last_step)
		{
			luaqueue->playing_next = luaqueue->start_step;
		}
		else
		{
			luaqueue->playing_next = (luaqueue->playing_next)->next;
		}
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


static void dmx_luaqueue_del_next_lock(struct dmx_luaqueuelock* lock)
{
	struct dmx_luaqueuelock* next_lock = lock->next;
			
	struct dmx_queue* queue = dmx_queue_getbyname(lock->name);
	if(queue != NULL)
	{
		dmx_queue_deactivate(queue);
	}
	free(lock->name);
	free(lock);

	if(next_lock != NULL)
	{
		dmx_luaqueue_del_next_lock(next_lock);
	}

}

static void dmx_luaqueue_del_next_step(struct dmx_luastep* step)
{
	struct dmx_luastep* next_step = step->next;

	free(step->code);
	free(step);

	if(next_step != NULL)
	{
		dmx_luaqueue_del_next_step(next_step);
	}
}

static void dmx_luaqueue_del_next(struct dmx_luaqueue* luaqueue)
{
	struct dmx_luaqueue* next_queue = luaqueue->next;

	free(luaqueue->name);
	
	if(luaqueue->firstlock != NULL)
		dmx_luaqueue_del_next_lock(luaqueue->firstlock);

	if(luaqueue->start_step != NULL)
		dmx_luaqueue_del_next_step(luaqueue->start_step);


	free(luaqueue);


	if(next_queue != NULL)
	{
		dmx_luaqueue_del_next(next_queue);
	}
}
void dmx_luaqueue_del_all(void)
{
	if(dmx_luaqueue_start == NULL) return;
	dmx_luaqueue_del_next(dmx_luaqueue_start);
}
