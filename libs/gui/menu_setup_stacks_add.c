
#include "menu.h"
#include "menu_setup_stacks_add.h"
//#include "screen_keyboard.h"i
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_stack.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct menu* menu_setup_stacks_add = NULL;


//static uint8_t init_device=1;

//static uint8_t invoke=0;

//#define PRE_ALLOCATE 8

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

static void menu_setup_stacks_add_redraw(void)
{
	/*
	   if(invoke != 0)
	   {
	   if(invoke==1)
	   {
	//printf("nameip\n");
	char* name = get_keyboard_buffer();
	my_free(new_device_class->name);
	new_device_class->name=my_malloc(strlen(name)+1);
	strcpy(new_device_class->name, name);
	}
	else if(invoke==2)
	{
	uint8_t ch = get_keyboard_number();
	if((ch > 0) &&(ch <= PRE_ALLOCATE))
	{
	new_device_class->channels = ch;
	}
	}
	else if(invoke>2)
	{
	if(invoke%2==1)
	{
	char* name = get_keyboard_buffer();
	my_free(new_device_class->channel_names[(invoke-2)>>1]);
	new_device_class->channel_names[(invoke-2)>>1]=my_malloc(strlen(name)+1);
	strcpy(new_device_class->channel_names[(invoke-2)>>1], name);
	}
	else
	{
	uint8_t def = get_keyboard_number();
	new_device_class->channel_defaults[(invoke-3)>>1] = def;

	}

	}
	invoke=0;
	}
	*/
	//	if(redraw)
	//	{
	clearDisplay();
	//		clear_buttons();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Add Stack");

	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Add Stack", 200,200,255);

	init_lineitems();
	draw_lineitem("Name","");


	draw_scrollbar();

	draw_button_h(257,45,52,42,"^",155,0,0,0,255,0);
	draw_button_h(257,92,52,42,"Edit",155,0,0,0,255,0);
	draw_button_h(257,139,52,42,"Save",155,0,0,0,255,0);
	draw_button_h(257,186,52,42,"v",155,0,0,0,255,0);



}

static void menu_setup_stacks_add_touch(unsigned int x, unsigned int y)
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
			if(menu_setup_stacks_add->parent != NULL)
				set_current_menu(menu_setup_stacks_add->parent);
			//
			//			redraw=1;
			//			init_device=1;
			//			for(int i=0;i<new_device_class->channels_allocated;i++)
			//			{
			//				my_free(new_device_class->channel_names[i]);
			//			}
			//			my_free(new_device_class->name);
			//			my_free(new_device_class->channel_defaults);
			//			my_free(new_device_class->channel_defaults_blackout);
			//			my_free(new_device_class->channel_defaults_identify);
			//			my_free(new_device_class->channel_defaults_fullbright);
			//			my_free(new_device_class->channel_names);
			//			my_free(new_device_class);
			//			set_current_execution(menu_setup_device_classes);
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
		//		redraw=1;
	}
	if(field == 4)
	{
		/*if(active_row < (new_device_class->channels*2+2)-1)
		{
			active_row++;
			int drawline = active_row-scroll_offset; 
			if(drawline==10)
			{
				scroll_offset++;
			}
		}*/
		//		redraw=1;
	}
	if(field == 2)
	{
		//		invoke=active_row+1;
		//		redraw=1;
		if(active_row==0)
		{
			//			invoke_keyboard("Enter Class Name",new_device_class->name);
		}
		else if(active_row==1)
		{
			//			invoke_numeric_keyboard("Number of Channels",new_device_class->channels);
		}
		else if(active_row>1)
		{
			if(active_row%2==0)
			{
				//				char strbuf[30];
				//				snprintf(strbuf,sizeof(strbuf),"Ch. %i Name",active_row>>1);
				//				//printf("inv: %s %i \n",strbuf,(active_row-2)>>1);
				//				invoke_keyboard(strbuf,new_device_class->channel_names[(active_row-2)>>1]);
			}else{
				//				char strbuf[30];
				//				snprintf(strbuf,sizeof(strbuf),"Ch. %i Default",(active_row-1)>>1);
				//				//printf("inv: %s %i \n",strbuf,(active_row-3)>>1);
				//				invoke_numeric_keyboard(strbuf,new_device_class->channel_defaults[(active_row-3)>>1]);
			}
		}
	}
	if(field == 3)
	{
		//		add_device_class(new_device_class);
		//		redraw=1;
		//		init_device=1;
		//		set_current_execution(menu_setup_device_classes);

	}
}


struct menu* get_menu_setup_stacks_add()
{
	if(menu_setup_stacks_add == NULL)
	{
		menu_setup_stacks_add = malloc(sizeof(struct menu));
		menu_setup_stacks_add->redraw = menu_setup_stacks_add_redraw;
		menu_setup_stacks_add->touch = menu_setup_stacks_add_touch;
		menu_setup_stacks_add->parent = NULL;
	}

	return menu_setup_stacks_add;
}



