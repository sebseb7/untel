#include <stdlib.h>
#include <string.h>

#include "pixel_hal.h"
#include "menu_list.h"

struct menu_list* menu_list_new(void)
{
	struct menu_list* menu_list = malloc(sizeof(struct menu_list));
	menu_list->length=0;
	menu_list->selected=0;
	menu_list->first = NULL;
	menu_list->last  = NULL;
	
	return menu_list;
}

struct menu_list_entry* menu_list_entry_new(unsigned int type,const char* label,unsigned int value1,unsigned int value2)
{
	struct menu_list_entry* menu_list_entry = malloc(sizeof(struct menu_list_entry));
	menu_list_entry->type = type;
	strncpy(menu_list_entry->label,label,DMX_NAME_LENGTH);
	menu_list_entry->value1=value1;
	menu_list_entry->value2=value2;

	return menu_list_entry;
}

void menu_list_add_entry(struct menu_list* menu, struct menu_list_entry* entry, signed int position)
{
	menu->length++;

	if(menu->first == NULL)
	{
		menu->first=entry;	
		menu->last=entry;	
	}

	if(position == -1)
	{
		menu->last->next=entry;
		menu->last = entry;
	}
}

