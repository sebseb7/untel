#ifndef _MENU_H
#define _MENU_H

#include "dmx_defines.h"

enum
{
	MENU_LIST_ENTRY_LABEL=0,
	MENU_LIST_ENTRY_DMX,
};


struct menu_list_entry
{
	unsigned int type;
	char label[DMX_NAME_LENGTH];
	unsigned int value1;
	unsigned int value2;

	struct menu_list_entry* next;
};

struct menu_list
{
	unsigned int length;
	unsigned int selected;
	struct menu_list_entry* first;
	struct menu_list_entry* last;
};

struct menu_list* menu_list_new(void);
struct menu_list_entry* menu_list_entry_new(unsigned int type,const char* label,unsigned int value1,unsigned int value2);
void menu_list_add_entry(struct menu_list* menu, struct menu_list_entry* entry, signed int position);


#endif
