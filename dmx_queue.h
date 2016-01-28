#ifndef _DMX_QUEUE_H
#define _DMX_QUEUE_H

struct dmx_queue 
{
	void (*init)(void);
	void (*deinit)(void);
	void (*tick)(unsigned int);
};

struct dmx_queue* dmx_queue_add(void (*init)(void),void (*deinit)(void),void (*tick)(unsigned int));
unsigned int dmx_queue_get_count(void);
struct dmx_queue* dmx_queue_getbyidx(unsigned int index);

#endif

