#ifndef _DMX_QUEUE_H
#define _DMX_QUEUE_H

struct dmx_queue 
{
	unsigned int active;
	void (*init)(void);
	void (*deinit)(void);
	unsigned int (*tick)(unsigned int);
	unsigned int next;
};

struct dmx_queue* dmx_queue_add(void (*init)(void),void (*deinit)(void),unsigned int (*tick)(unsigned int));
unsigned int dmx_queue_get_count(void);
struct dmx_queue* dmx_queue_getbyidx(unsigned int index);
void dmx_queue_activate(struct dmx_queue* queue);
void dmx_queue_deactivate(struct dmx_queue* queue);
#endif

