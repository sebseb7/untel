#ifndef _DMX_QUEUE_H
#define _DMX_QUEUE_H

struct dmx_queue 
{
	void (*init)(void);
	void (*deinit)(void);
	void (*step)(unsigned int);
};

struct dmx_queue* dmx_queue_add(void (*init)(void),void (*deinit)(void),void (*step)(unsigned int));

#endif

