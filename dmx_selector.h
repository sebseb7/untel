#ifndef _DMX_SELECTOR_H
#define _DMX_SELECTOR_H

#include "dmx_devices.h"

struct dmx_selector
{
	unsigned int length;
	void (*render_selection)(unsigned int);
	void (*getname)(unsigned int,char*);
	void (*init)(void);
	void (*deinit)(void);
	
	unsigned int dev_alloc;
	unsigned int dev_count;
	struct dmx_device* dev_list;
	
	unsigned int sets_alloc;
	unsigned int set_count;
	struct dmx_set* set_list;

};

struct dmx_selector * dmx_selector_add(void*(void),void*(void),void(unsigned int,char*),void(unsigned int));
void dmx_selector_add_setCol(struct dmx_selector* selector,unsigned char red,unsigned char green,unsigned char blue);
void dmx_selector_add_setDim(struct dmx_selector* selector,float value);
void dmx_selector_add_device(struct dmx_selector* selector,unsigned int type, char* name);



void dmx_selector_del(struct dmx_selector* selector);
unsigned int dmx_selector_get_count(void);
struct dmx_selector* dmx_selector_getbyidx(unsigned int index);

void dmx_selector_render(struct dmx_selector* dmx_selector);
void dmx_selector_deactivate(struct dmx_selector* dmx_selector);
void dmx_selector_set(struct dmx_selector* dmx_selector,unsigned int idx);


#endif
