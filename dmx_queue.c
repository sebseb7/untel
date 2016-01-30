#include <stdlib.h>
#include <string.h>

#include "dmx_queue.h"



#define DMX_QUEUE_ALLOCATE_INITIAL 100
#define DMX_QUEUE_ALLOCATE_STEP 10

static struct dmx_queue** dmx_queue_list;

static unsigned int queues_allocated=0;
static unsigned int queues_inuse=0;

unsigned int dmx_queue_get_count(void)
{
	return queues_inuse;
}

struct dmx_queue* dmx_queue_getbyidx(unsigned int index)
{
	if(index < queues_inuse)
	{
		return dmx_queue_list[index];
	}
	return NULL;
}



struct dmx_queue* dmx_queue_add(char* name,void (*init)(void),void (*deinit)(void),unsigned int (*tick)(unsigned int))
{
	if(0==queues_allocated)
	{
		dmx_queue_list = malloc(sizeof(*dmx_queue_list)*DMX_QUEUE_ALLOCATE_INITIAL);
		queues_allocated=DMX_QUEUE_ALLOCATE_INITIAL;
	}

	if(queues_inuse == queues_allocated)
	{
		//implement realloc
		exit(EXIT_FAILURE);
	}
	struct dmx_queue* queue = malloc(sizeof(*queue));
	dmx_queue_list[queues_inuse]=queue;
	queues_inuse++;

	strncpy(queue->name,name,DMX_NAME_LENGTH);
	queue->active = 0;
	queue->init = init;
	queue->deinit = deinit;
	queue->tick = tick;
	queue->next = 0;

	return queue;
}
void dmx_queue_activate(struct dmx_queue* queue)
{
	queue->active++;
}
void dmx_queue_deactivate(struct dmx_queue* queue)
{
	if(queue->active!=0)
		queue->active--;
}
struct dmx_queue* dmx_queue_getbyname(char* name)
{	
	for(unsigned int i=0;i<queues_inuse;i++)
	{
		if(0==strncmp(dmx_queue_list[i]->name,name,DMX_NAME_LENGTH))
		{
			return dmx_queue_list[i];
		}
	}
	return NULL;
}

