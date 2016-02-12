#include <stdio.h>

#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"

static unsigned int active = 0;
static unsigned int step = 0;

static void init(void)
{
}

static void deinit(void)
{
	if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SLIDE"));
	if(active == 2) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SWITCH"));
	active=0;
	step=0;
}



static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	switch(step++)
	{
		case 0:
			if(active == 1) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SLIDE"));
			if(active != 2) dmx_queue_activate(dmx_queue_getbyname("LED-COL-SWITCH"));
			active=2;
			return 200000;
		default:
			if(active != 1) dmx_queue_activate(dmx_queue_getbyname("LED-COL-SLIDE"));
			if(active == 2) dmx_queue_deactivate(dmx_queue_getbyname("LED-COL-SWITCH"));
			active=1;
			step=0;
			return 200000;
	}
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_add("LED-COL-TRIG",init,deinit,tick);

}


