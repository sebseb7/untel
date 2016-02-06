#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static unsigned int active = 0;

static void init(void)
{
}

static void deinit(void)
{
	if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-DIM-SEQ1"));
	if(active == 2) dmx_queue_deactivate(dmx_queue_getbyname("LED-DIM-SEQ2"));
	active=0;
}


static unsigned int step = 0;

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-DIM-SEQ1"));
			if(active != 2) dmx_queue_activate(dmx_queue_getbyname("LED-DIM-SEQ2"));
			active=2;
			return 30000;
		default:
			if(active != 1) dmx_queue_activate(dmx_queue_getbyname("LED-DIM-SEQ1"));
			if(active == 2) dmx_queue_deactivate(dmx_queue_getbyname("LED-DIM-SEQ2"));
			active=1;
			step=0;
			return 30000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_queue* queue = dmx_queue_add("LED-DIM-SEQ",init,deinit,tick);
	dmx_queue_activate(queue);

}


