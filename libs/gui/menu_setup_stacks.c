
#include "menu.h"
#include "menu_setup.h"
#include "menu_setup_stacks.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_stack.h"

#include <stdio.h>
#include <string.h>

static struct menu* menu_setup_stacks = NULL;

static uint8_t active_row = 0;
static uint8_t scroll_offset = 0;

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

static uint8_t current_lineitem=0;

static void init_lineitems(void)
{
	current_lineitem=0;
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

static void menu_setup_stacks_redraw(void)
{
	clearDisplay();
	//clear_buttons();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Setup Stacks");

	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Setup Stacks", 200,200,255);


	init_lineitems();
	char buf[30];
	char buf2[30];
	draw_lineitem("","<new>");
	for(unsigned int i = 0; i < dmx_stack_get_count();i++)
	{
		//struct dmx_image* dmx_image = dmx_image_getbyidx(i);

		snprintf(buf,30,"cat");
		snprintf(buf2,30,"name");
		draw_lineitem(buf,buf2);
	}

	draw_scrollbar();

	draw_button_h(257,45,52,42,"^",155,0,0,0,255,0);
	if((dmx_get_device_count()<10)&&(active_row==0))
	{
		draw_button_h(257,92,52,42,"Add",155,0,0,0,255,0);
	}
	if(active_row>0)
	{
		draw_button_h(257,92,52,42,"Edit",155,0,0,0,255,0);
		draw_button_h(257,139,52,42,"Delete",155,0,0,0,255,0);
	}
	draw_button_h(257,186,52,42,"v",155,0,0,0,255,0);
}

static void menu_setup_stacks_touch(unsigned int x, unsigned int y)
{

	uint8_t field=0;

	if(y > 41)
	{
		field=1;

		if(y > 184)
		{
			field=4;
		}else if(y > 136)
		{
			field=3;
		}else if(y > 89)
		{
			field=2;
		}
	}
	else
	{
		if(x < 40)
		{
			if(menu_setup_stacks->parent != NULL)
				set_current_menu(menu_setup_stacks->parent);
		}
	}

	if(field == 1)
	{
		if(active_row > 0)
		{
			if(active_row==scroll_offset)
			{
				scroll_offset--;
			}

			active_row--;
		}
		set_menu_dirty();
	}
	if(field == 4)
	{
		if(active_row < (dmx_stack_get_count()))
		{
			active_row++;
			int drawline = active_row-scroll_offset; 
			if(drawline==10)
			{
				scroll_offset++;
			}
		}
		set_menu_dirty();
	}
	if(field == 2)
	{
		//			redraw=1;
		if(active_row==0)
		{
			if(dmx_stack_get_count()<50)
			{
				//			set_current_execution(menu_setup_devices_add);
			}
		}
		else
		{
			//		set_current_execution(menu_setup_devices_edit);
			//		set_device_to_edit(active_row-1);
		}
	}
	if(field == 3)
	{
		//			redraw=1;

		if(active_row > 0)
		{
			//del_device(active_row-1);
			//if(active_row==scroll_offset)
			//{
			//	scroll_offset--;
			//}
			//active_row--;
		}

	}
}

struct menu* get_menu_setup_stacks()
{
	if(menu_setup_stacks == NULL)
	{
		menu_setup_stacks = malloc(sizeof(struct menu));
		menu_setup_stacks->redraw = menu_setup_stacks_redraw;
		menu_setup_stacks->touch = menu_setup_stacks_touch;
		menu_setup_stacks->parent = NULL;
	}

	return menu_setup_stacks;
}


