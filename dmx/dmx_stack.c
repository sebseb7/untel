#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_stack.h"

#define DMX_STACK_FRAMES_ALLOCATE_INITIAL 100
#define DMX_STACK_ALLOCATE_INITIAL 100
#define DMX_STACK_ALLOCATE_STEP 10

static struct dmx_stack** dmx_stack_list;

//static unsigned int dmx_stack_allocated=0;
static unsigned int dmx_stack_inuse=0;

struct dmx_stack* dmx_stack_new(void)
{
	struct dmx_stack* stack = malloc(sizeof(struct dmx_stack));

	strcpy(stack->category,"");
	strcpy(stack->name,"");
	stack->active=0;
	stack->length=0;
	stack->alloc=DMX_STACK_FRAMES_ALLOCATE_INITIAL;
	stack->frames=malloc(sizeof(dmx_frame*)*DMX_STACK_FRAMES_ALLOCATE_INITIAL);

	return stack;
}

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

void dmx_stack_add_imgframe(struct dmx_stack* stack,struct dmx_img* img)
{
	if(stack->length == stack->alloc)
	{
		printf("stf2\n");
		exit(EXIT_FAILURE);
	}

	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type=DMX_FRAME_IMAGE;
	frame->image.image=img;

	stack->frames[stack->length]=frame;
	stack->length++;
}
void dmx_stack_del_imgframe(struct dmx_stack* stack,unsigned int idx)
{
	if(idx >= stack->length)
		return;

	dmx_frame* frame = stack->frames[idx];
				
	for (unsigned int c = idx ; c < stack->length - 1 ; c++ )
	{
		stack->frames[c] = stack->frames[c+1];
	}
	stack->frames[stack->length]=NULL;

	if(frame->type == DMX_FRAME_IMAGE)
	{
		dmx_img_free(frame->image.image);
	}
	free(frame);

	stack->length--;
}
unsigned int dmx_stack_frame_count(struct dmx_stack* stack)
{
	return stack->length;
}

dmx_frame* dmx_stack_frame_getbyidx(struct dmx_stack* stack,unsigned int idx)
{
	if(idx < stack->length)
	{
		return stack->frames[idx];
	}
	return NULL;
}

