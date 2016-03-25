#include <stdlib.h>
#include <string.h>

#include "pixel_hal.h"
#include "menu_list.h"
#include "mcugui/button.h"

/*
static void draw_background(uint8_t drawrow,uint8_t row)
{
	if(row == active_row)
	{
		draw_filledRect(6,45+(drawrow*18),200,14,155,100,100);
	}
	else
	{
		draw_filledRect(6,45+(drawrow*18),200,14,60,60,60);
	}
}

static void draw_lineitem(char* name,char* value)
{
	if((current_lineitem < scroll_offset+10)&&(current_lineitem>=scroll_offset))
	{
		draw_background(current_lineitem-scroll_offset,current_lineitem);
		draw_text_8x6(10,45+(current_lineitem-scroll_offset)*18,name,255,255,0);
		draw_text_8x6(80,45+(current_lineitem-scroll_offset)*18,value,255,0,255);
	}
	current_lineitem++;
}
static void draw_scrollbar(void)
{
	if(current_lineitem>10)
	{
		draw_filledRect(230,45,10,176,50,50,50);
		float perline = 176.0/(float)current_lineitem;
		draw_filledRect(230,45+(scroll_offset*perline),10,perline*10.0,50,150,50);

	}
}
*/

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

	while(current != NULL)
	{

		current = current->next;
	}




//	draw_scrollbar();

	draw_button_h(x+100,y+45,52,42,"^",155,0,0,0,255,0);
	draw_button_h(x+100,y+92,52,42,"Edit",155,0,0,0,255,0);
	draw_button_h(x+100,y+139,52,42,"Save",155,0,0,0,255,0);
	draw_button_h(x+100,y+186,52,42,"v",155,0,0,0,255,0);

}

