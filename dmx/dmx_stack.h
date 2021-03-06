#ifndef _DMX_STACK_H
#define _DMX_STACK_H

#include "dmx_defines.h"
#include "dmx_frame.h"


struct dmx_stack 
{
	char name[DMX_NAME_LENGTH];
	char group[DMX_NAME_LENGTH];

	unsigned int active;
	unsigned int blendpointer;
	unsigned long long lastframetime;

	unsigned int length;
	unsigned int alloc;

	dmx_frame** frames;
};

struct dmx_stack* dmx_stack_new(void);
void dmx_frame_free(dmx_frame* frame);
void dmx_stack_delete(unsigned int index);
unsigned int dmx_stack_get_count(void);
struct dmx_stack* dmx_stack_getbyidx(unsigned int index);
struct dmx_stack* dmx_stack_getbyname(char* name);
struct dmx_stack* dmx_stack_getactiveingroup(char* group);
struct dmx_stack* dmx_stack_clone(struct dmx_stack*);
void dmx_stack_free(struct dmx_stack*);
void dmx_stack_store(struct dmx_stack*);
void dmx_stack_store_to_disc(char * filename);
//void dmx_stack_store_to_disc(void);
void dmx_stack_load_from_disc(void);

unsigned int dmx_stack_frame_count(struct dmx_stack* stack);			

dmx_frame* dmx_frame_newimg(struct dmx_img* img);
void dmx_stack_add_imgframe(struct dmx_stack* stack,struct dmx_img* img);
void dmx_stack_add_imgframe_atidx(struct dmx_stack* stack,struct dmx_img* img,signed int idx);
void dmx_stack_del_imgframe(struct dmx_stack* stack,unsigned int idx);

dmx_frame* dmx_frame_newwait(unsigned int type,unsigned int value,unsigned int blend);
void dmx_stack_add_waitframe(struct dmx_stack* stack,unsigned int type,unsigned int value,unsigned int blend);
void dmx_stack_add_waitframe_atidx(struct dmx_stack* stack,unsigned int type,unsigned int value,unsigned int blend,signed int idx);

dmx_frame* dmx_stack_frame_getbyidx(struct dmx_stack* stack,unsigned int idx);

void dmx_stack_process(struct dmx_stack*);

#endif
