#include <stdlib.h>
#include <string.h>

#include "pixel_hal.h"
#include "menu_list.h"
#include "mcugui/button.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"

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
	menu_list_entry->next = NULL;

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
	else if(position == -1)
	{
		menu->last->next=entry;
		menu->last = entry;
		entry->next=NULL;
	}
}

void menu_draw(struct menu_list* menu,unsigned int x,unsigned int y,unsigned int size)
{
	struct menu_list_entry* current = menu->first;

	if(size > 10) size = 10;

	unsigned int pos=0;

	while(current != NULL)
	{
		if(menu->selected == pos)
		{
			draw_filledRect(x,y+(pos*18),226,14,155,100,100);
		}
		else
		{
			draw_filledRect(x,y+(pos*18),226,14,60,60,60);
		}
		draw_text_8x6(x+4,y+(pos*18),current->label,255,255,0);

		current = current->next;
		pos++;
	}

	if(pos > 0)
	{
		draw_filledRect(x+230,y,10,176,50,50,50);
		float perline = 176.0/(float)pos;
		draw_filledRect(x+230,y+(perline),10,perline*10.0,50,150,50);
	}

	draw_button_h(x+245,y,52,42,"^",155,0,0,0,255,0);
	draw_button_h(x+245,y+47,52,42,"Edit",155,0,0,0,255,0);
	draw_button_h(x+245,y+94,52,42,"Save",155,0,0,0,255,0);
	draw_button_h(x+245,y+141,52,42,"v",155,0,0,0,255,0);

}

