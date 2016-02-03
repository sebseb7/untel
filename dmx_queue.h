#ifndef _DMX_QUEUE_H
#define _DMX_QUEUE_H

#include "dmx_devices.h"

struct dmx_queue 
{
	unsigned int active;
	char name[DMX_NAME_LENGTH];
	void (*init)(void);
	void (*deinit)(void);
	unsigned int (*tick)(unsigned int);
	unsigned int next;
};

struct dmx_queue* dmx_queue_add(char* name,void (*init)(void),void (*deinit)(void),unsigned int (*tick)(unsigned int));
unsigned int dmx_queue_get_count(void);
struct dmx_queue* dmx_queue_getbyidx(unsigned int index);
struct dmx_queue* dmx_queue_getbyname(char* name);
void dmx_queue_activate(struct dmx_queue* queue);
void dmx_queue_deactivate(struct dmx_queue* queue);

void dmx_queues_print(void);

#endif

