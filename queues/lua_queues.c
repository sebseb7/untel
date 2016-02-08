#include "main.h"
#include "dmx_luaqueue.h"

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_luaqueue* luaqueue = dmx_luaqueue_add("lua1");

	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-DIM-SEQ_ON\")\nqueue_on( \"LED-COL-SLIDE\")\nqueue_off(\"LED-COL-SWITCH\")",30000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_off(\"LED-COL-SLIDE\")\nqueue_on( \"LED-COL-SWITCH\")",30000,0,0,0);

	dmx_luaqueue_activate(luaqueue);

}
