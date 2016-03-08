#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_stack.h"

#define DMX_STACK_ALLOCATE_INITIAL 100
#define DMX_STACK_ALLOCATE_STEP 10

static struct dmx_stack** dmx_stack_list;

static unsigned int dmx_stack_allocated=0;
static unsigned int dmx_stack_inuse=0;

unsigned int dmx_stack_get_count(void)
{
	return dmx_stack_inuse;
}
struct dmx_stack* dmx_stack_getbyidx(unsigned int index)
{
	if(index < dmx_stack_inuse)
	{
		return dmx_stack_list[index];
	}
	return NULL;
}

