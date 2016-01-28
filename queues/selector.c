#include "main.h"
#include "dmx_queue.h"
#include "dmx_devices.h"
#include "dmx_image.h"
#include "dmx_selector.h"

static void init(void)
{
}

static void deinit(void)
{
}

static void getname(__attribute__((__unused__)) unsigned int idx,__attribute__((__unused__)) char* name)
{
}


static void position(__attribute__((__unused__)) unsigned int time)
{
}


static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	
	dmx_device_create_ledpar(1,0,"bar");

	struct dmx_selector* selector = dmx_selector_add("LP COL",init,deinit,getname,position,1);

	dmx_selector_add_device(selector,DMX_DEVICE_LEDPAR,"bar");
	dmx_selector_set(selector,1);

}


