#include "main.h"
#include "dmx_luaqueue.h"

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
static void constructor(void) {

	struct dmx_luaqueue* luaqueue = dmx_luaqueue_add("lua-par56");

	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-006\")\nqueue_off(\"LED-SCENE-001\")",26*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-005\")\nqueue_off(\"LED-SCENE-006\")",26*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-004\")\nqueue_off(\"LED-SCENE-005\")",25*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-002\")\nqueue_off(\"LED-SCENE-004\")",25*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-003\")\nqueue_off(\"LED-SCENE-002\")",25*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue,"queue_on(\"LED-SCENE-001\")\nqueue_off(\"LED-SCENE-003\")",26*10000,0,0,0);

	dmx_luaqueue_activate(luaqueue);



	struct dmx_luaqueue* luaqueue2 = dmx_luaqueue_add("lua-strobe");

	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-003\")\nqueue_on(\"STROBE-SCENE-002\")",9*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-002\")\nqueue_on(\"STROBE-SCENE-003\")",9*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-003\")\nqueue_on(\"STROBE-SCENE-001\")",6*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-001\")\nqueue_on(\"STROBE-SCENE-003\")",9*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-003\")\nqueue_on(\"STROBE-SCENE-004\")",9*10000,0,0,0);
	dmx_luaqueue_addstep(luaqueue2,"queue_off(\"STROBE-SCENE-004\")\nqueue_on(\"STROBE-SCENE-003\")",9*10000,0,0,0);

	dmx_luaqueue_activate(luaqueue2);

}
