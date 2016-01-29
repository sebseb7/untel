#include <stdlib.h>

#include "dmx_queue.h"



#define DMX_QUEUE_ALLOCATE_INITIAL 100
#define DMX_QUEUE_ALLOCATE_STEP 10

static struct dmx_queue* dmx_queue_list;

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
		return &dmx_queue_list[index];
	}
	return NULL;
}



struct dmx_queue* dmx_queue_add(void (*init)(void),void (*deinit)(void),unsigned int (*tick)(unsigned int))
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
	struct dmx_queue* queue = &dmx_queue_list[queues_inuse];
	queues_inuse++;

	queue->init = init;
	queue->deinit = deinit;
	queue->tick = tick;
	queue->next = 0;

	return queue;
}

