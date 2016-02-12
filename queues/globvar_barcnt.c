#include <stdio.h>

#include "main.h"

#include "dmx_queue.h"
#include "dmx_globvar.h"

static unsigned int beat = 0;
static unsigned int bar = 0;

static void init(void)
{
	beat = 0;
	bar = 0;
}


static void deinit(void)
{
}


static unsigned int tick(__attribute__((__unused__)) unsigned int time)
{
	beat++;
	if(beat==4)
	{
		beat=0;
		bar++;
	}
//	printf("barcnt %i %i %i\n",time,bar,beat);

	dmx_globvar_set("barcnt",bar);

	return time;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	dmx_queue_activate(dmx_queue_add("GLOB-BARCNT",init,deinit,tick));

}
