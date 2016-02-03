#ifndef _DMX_SELECTOR_H
#define _DMX_SELECTOR_H

#include "dmx_image.h"

struct dmx_selector
{
	char name[DMX_NAME_LENGTH];
	unsigned int length;
	void (*render)(unsigned int);
	void (*getname)(unsigned int,char*);
	unsigned int (*getidbyname)(char*);
	void (*init)(void);
	void (*deinit)(void);
	
	
	unsigned int sets_alloc;
	unsigned int set_count;
	struct dmx_set** set_list;
};

struct dmx_selector * dmx_selector_add(char* name,void(*init)(void),void(*deinit)(void),void(*getname)(unsigned int,char*),unsigned int(*getidbyname)(char *),void(*render)(unsigned int),unsigned int length);
void dmx_selector_attach_set(struct dmx_selector* selector,struct dmx_set* set);


void dmx_selector_del(struct dmx_selector* selector);
unsigned int dmx_selector_get_count(void);
struct dmx_selector* dmx_selector_getbyidx(unsigned int index);
struct dmx_selector* dmx_selector_getbyname(char* name);

void dmx_selector_print(void);


#endif
