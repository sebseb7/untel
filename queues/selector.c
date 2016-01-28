#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_selector.h"

static void init(void)
{
	dmx_device_create_ledpar(1,0,"bar");
}

static void deinit(void)
{
}

static void getname(__attribute__((__unused__)) unsigned int idx,char* name)
{
}


static void position(__attribute__((__unused__)) unsigned int time)
{
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {

	dmx_selector_add(init,deinit,getname,position);

}


