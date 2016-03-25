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
	menu_list->offset=0;
	menu_list->first = NULL;
	menu_list->last  = NULL;
	menu_list->touchlist = NULL;

	return menu_list;
}

void menu_list_free(struct menu_list* menu)
{
	if(menu->touchlist != NULL)
		touch_binding_free(menu->touchlist);
	
	struct menu_list_entry* current = menu->first;
	
	while(current != NULL)
	{
		struct menu_list_entry* next = current->next;
		menu_list_entry_free(current);
		current=next;
	}
	free(menu);
}

void menu_list_entry_free(struct menu_list_entry* entry)
{
	free(entry);
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

void menu_list_draw(struct menu_list* menu,unsigned int x,unsigned int y,unsigned int size)
{
	struct menu_list_entry* current = menu->first;

	if(size < 10) size = 10;

	unsigned int pos=0;
	unsigned int drawn=0;
	while((current != NULL)&&(drawn < size))
	{
		if(pos >= menu->offset)
		{
			if((menu->selected + menu->offset) == pos)
			{
				draw_filledRect(x,y+((pos - menu->offset)*18),226,14,155,100,100);
			}
			else
			{
				draw_filledRect(x,y+((pos - menu->offset)*18),226,14,60,60,60);
			}
			draw_text_8x6(x+4,y+((pos - menu->offset)*18),current->label,255,255,0);
			drawn++;
		}
		pos++;

		current = current->next;
	}

	if(pos > 0)
	{
		draw_filledRect(x+230,y,10,176,50,50,50);
		float perline = 176.0/(float)menu->length;
		if(menu->length > (size-1))
			draw_filledRect(x+230,y+(menu->offset*perline),10,perline*size,50,150,50);
	}
	
	if(menu->touchlist != NULL)
		touch_binding_free(menu->touchlist);
	menu->touchlist = touch_binding_new();

	touch_binding_add(menu->touchlist,245,52,0,42,1,0,0);
	touch_binding_add(menu->touchlist,245,52,141,42,4,0,0);

	draw_button_h(x+245,y,52,42,"^",155,0,0,0,255,0);
	//draw_button_h(x+245,y+47,52,42,"Edit",155,0,0,0,255,0);
	//draw_button_h(x+245,y+94,52,42,"Save",155,0,0,0,255,0);
	draw_button_h(x+245,y+141,52,42,"v",155,0,0,0,255,0);

}

signed int menu_list_touch(struct menu_list* menu,unsigned int x,unsigned int y)
{
	if(menu != NULL)
	{
		unsigned int attr1 = 0;
		unsigned int attr2 = 0;
		unsigned int attr3 = 0;
		unsigned int relx = 0;
		unsigned int rely = 0;
		if(touch_test(menu->touchlist,x,y,&attr1,&attr2,&attr3,&relx,&rely))
		{
			if(attr1 == 1)
			{
				if((menu->selected + menu->offset) > 0)
				{
					if(menu->selected > 0)
					{
						menu->selected--;
					}
					else
					{
						menu->offset--;
					}
				}
				return 0;
			}
			else if(attr1 == 4)
			{
				if((menu->selected + menu->offset) < (menu->length -1))
				{
					if(menu->selected < 9)
					{
						menu->selected++;
					}
					else
					{
						menu->offset++;
					}
				}
				return 0;
			}
		}
	}
	return -1;
}

