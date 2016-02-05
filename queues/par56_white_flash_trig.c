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
	if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-WHITEFLASH"));
	active=0;
}


static unsigned int step = 0;

static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-WHITEFLASH"));
			active=0;
			return 33300;
		default:
			if(active != 1) dmx_queue_activate(dmx_queue_getbyname("LED-WHITEFLASH"));
			active=1;
			step=0;
			return 2000;
	}


	return 0;
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_queue* queue = dmx_queue_add("LED-WHITEFLASH-TRIG",init,deinit,tick);
	dmx_queue_activate(queue);

}


