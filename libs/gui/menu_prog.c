#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

#include "screen_keyboard.h"
			//char* name = get_keyboard_buffer();

#include "menu.h"
#include "screen_keyboard.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"
#include "mcugui/circle.h"

#include "dmx_devices.h"
#include "dmx_frame.h"
#include "dmx_stack.h"

#include "dmx_attr_colors.h"

static struct menu* menu_prog = NULL;

static unsigned int tab = 0;

static struct dmx_img* stash[4];

static struct dmx_stack* prog_stack = NULL;

static struct menu_list* list1 = NULL;

/*
 *
 * one of the 4 stashes is the "current" one
 * stashes can be visible or hidden
 * each stash can be added to a stack or replace a current entry
 *
 */

#include "touch_binding.h"
#include "menu_list.h"


static struct touch_binding_list* touchlist = NULL;


static unsigned int act = 0;
static unsigned int act_frame_type = DMX_FRAME_IMAGE;
static unsigned int act_attribute = 0;
static unsigned int act_wait_type = 0;
static unsigned int act_wait_value = 0;

static unsigned int loop_enable = 0;

static const unsigned int listsize = 15;

static void menu_prog_redraw(void)
{
	clearDisplay();
	if(touchlist != NULL)
		touch_binding_free(touchlist);
	touchlist = touch_binding_new();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("Programmer");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Programmer", 200,200,255);

	unsigned int buttonx = 0;
	unsigned int buttony = 0;

	unsigned int devicecount = dmx_img_device_count(stash[act]);

	char *frame_str;
	if(act_frame_type == DMX_FRAME_IMAGE)
	{
		frame_str = "Image";
	}
	else if(act_frame_type == DMX_FRAME_WAIT)
	{
		frame_str = "Wait";
	}
	else
	{
		frame_str = "<>";
	}
	char *attr_str;
	if(act_attribute == DMX_ATTR_COLOR_NAME)
	{
		attr_str = "Color Name";
	}	
	else if(act_attribute == DMX_ATTR_COLOR_RGB)
	{
		attr_str = "Color RGB";
	}	
	else if(act_attribute == DMX_ATTR_DIM)
	{
		attr_str = "Dimmer";
	}	
	else
	{
		attr_str = "<>";
	}	

	if((loop_enable==1)&&((list1 == NULL)||(list1->length==0)))
	{
		loop_enable=0;
		set_programmer_loop(0);
	}



	touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,1,0);
	draw_button_icon2(button_x(buttonx++),button_y(buttony),92,1,"Frame",frame_str,155,(tab==1)?155:0,0,0,255,0,255,255,0);

	if(act_frame_type == DMX_FRAME_IMAGE)
	{
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,2,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Devices",       155,(tab==2)?155:0,0,(devicecount>0)?255:0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,3,0);
		draw_button_icon2(button_x(buttonx++),button_y(buttony),92,1,"Attribute",attr_str,155,(tab==3)?155:0,0,(act_attribute)?255:0,255,0,255,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,4,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Value",         155,(tab==4)?155:0,0,(act_attribute)?155:0,255,0);
	}
	else if(act_frame_type == DMX_FRAME_WAIT)
	{
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,1,5,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Value",         155,(tab==5)?155:0,0,255,255,0);
	}


	unsigned int validimg=0;
	if(
		(
			(act_frame_type == DMX_FRAME_IMAGE)&&
			(devicecount>0)&&
			(
				dmx_img_isdim(stash[act])||
				dmx_img_iscolname(stash[act])||
				dmx_img_iscolrgb(stash[act])
			)
		)||(
			(act_frame_type == DMX_FRAME_WAIT)&&(act_wait_value > 0)
		))
	{
		validimg=1;
		touch_binding_add(touchlist,button_x(5),92,button_y(0),54,2,0,0);//add
		if((list1 != NULL)&&(list1->length>0))
		{
			touch_binding_add(touchlist,button_x(5),92,button_y(1),54,8,0,0);//replace
		}
	}
	if((list1 != NULL)&&(list1->length>0))
	{
		touch_binding_add(touchlist,button_x(6),92,button_y(0),54,9,0,0);//delete
		touch_binding_add(touchlist,button_x(7),92,button_y(1),54,11,0,0);//store
		touch_binding_add(touchlist,button_x(7),92,button_y(0),54,14,0,0);//loop
	}

	draw_button_icon(button_x(5),button_y(0),92,1,"Add",(validimg)?155:55,0,0,0,(validimg)?255:55,0);
	draw_button_icon(button_x(5),button_y(1),92,1,"Replace",((validimg)&&(list1 != NULL)&&(list1->length>0))?155:55,0,0,0,((validimg)&&(list1 != NULL)&&(list1->length>0))?255:55,0);
	draw_button_icon(button_x(6),button_y(0),92,1,"Delete",((list1 != NULL)&&(list1->length>0))?155:55,0,0,0,((list1 != NULL)&&(list1->length>0))?255:55,0);
	if((list1 != NULL)&&(list1->length>0))
	{
		draw_button_icon(button_x(7),button_y(0),92,1,"Loop",155,(loop_enable==1)?155:0,0,(loop_enable==1)?255:0,255,0);
	}
	else
	{
		draw_button_icon(button_x(7),button_y(0),92,1,"Loop",55,0,0,0,55,0);
	}
	draw_button_icon(button_x(7),button_y(1),92,1,"Store",((list1 != NULL)&&(list1->length>0))?155:55,0,0,0,((list1 != NULL)&&(list1->length>0))?255:55,0);

	buttony=2;buttonx=0;

	if(tab==1)
	{
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,12,DMX_FRAME_IMAGE,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Image",155,(act_frame_type == DMX_FRAME_IMAGE)?155:0,0,55,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,12,DMX_FRAME_WAIT,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Wait",155,(act_frame_type == DMX_FRAME_WAIT)?155:0,0,55,255,0);
	}
	else if(tab==2)
	{
		char buf[30];
		char buf2[30];

		for(unsigned int i = 0; i < dmx_get_device_count();i++)
		{
			struct dmx_device* dmx_device = dmx_get_device_byidx(i);

			snprintf(buf,30,"%i",dmx_device->addr);

			char classname[30];

			if(dmx_device->type == DMX_DEVICE_LEDPAR6) 
			{
				strncpy(classname,"Par56LED",30);
			}
			else if(dmx_device->type == DMX_DEVICE_STROBE)
			{
				strncpy(classname,"Strobe",30);
			}
			else
			{
				strncpy(classname,"<missing>",30);
			}

			snprintf(buf2,30,"%s",dmx_device->name);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,3,i,0);
			unsigned int active = dmx_img_device_test(stash[act],dmx_device->name);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf2,155,(active)?155:0,0,0,255,0);
			if(buttonx == 8)
			{
				buttony++;
				buttonx=0;
			}
		}
	}
	else if(tab==3)
	{
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,10,DMX_ATTR_COLOR_NAME,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Color Name",155,(act_attribute==DMX_ATTR_COLOR_NAME)?155:0,0,(dmx_img_iscolname(stash[act]))?255:0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,10,DMX_ATTR_COLOR_RGB,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Color RGB",155,(act_attribute==DMX_ATTR_COLOR_RGB)?155:0,0,(dmx_img_iscolrgb(stash[act]))?255:0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,10,DMX_ATTR_DIM,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Dimmer",155,(act_attribute==DMX_ATTR_DIM)?155:0,0,(dmx_img_isdim(stash[act]))?255:0,255,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Frequency",55,(act_attribute==100)?55:0,0,0,55,0);
		buttony++;buttonx=0;
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Position",55,(act_attribute==100)?55:0,0,0,55,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Gobo",55,(act_attribute==100)?55:0,0,0,55,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Rotation",55,(act_attribute==100)?55:0,0,0,55,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Special",55,(act_attribute==100)?55:0,0,0,55,0);
	}
	else if(tab==4)
	{

		if(act_attribute == DMX_ATTR_COLOR_NAME)
		{

			unsigned int dmx_attr_colors_get_count(void);
			const char* dmx_attr_colors_get_name(unsigned int idx);
			char buf[30];

			unsigned int initial_buttonx = buttonx;

			for(unsigned int i = 0; i < dmx_attr_colors_get_count();i++)
			{
				const char * colorname = dmx_attr_colors_get_name(i);

				snprintf(buf,30,"%s",colorname);


				unsigned int active = dmx_img_color_test(stash[act],colorname);

				touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,4,i,0);
				draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,buf,155,(active)?155:0,0,0,255,0);

				if(buttonx == 5)
				{
					buttony++;
					buttonx=initial_buttonx;
				}
			}
		}
		if(act_attribute == DMX_ATTR_COLOR_RGB)
		{
			unsigned int height = button_y(5)-button_y(0)-11;
			unsigned int runlength = height-54;

			unsigned int r=0;
			unsigned int g=0;
			unsigned int b=0;

			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,1,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,2,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,5,3,0);
			draw_rect(button_x(buttonx++),button_y(buttony),92,height,1,155,0,0);
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,5,4,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,0,0,0,255,0);

			if(dmx_img_color_getrgb(stash[act],&r,&g,&b))
			{	
				unsigned int position_r = runlength * (1-(r/255.0f));
				draw_filledRect(button_x(buttonx-4)+1,button_y(buttony)+position_r+1,92-2,54-2,155,50,50);
				unsigned int position_g = runlength * (1-(g/255.0f));
				draw_filledRect(button_x(buttonx-3)+1,button_y(buttony)+position_g+1,92-2,54-2,50,155,50);
				unsigned int position_b = runlength * (1-(b/255.0f));
				draw_filledRect(button_x(buttonx-2)+1,button_y(buttony)+position_b+1,92-2,54-2,50,50,155);
			}
		}

		if(act_attribute == DMX_ATTR_DIM)
		{
			unsigned int height = button_y(5)-button_y(0)-11;
			unsigned int runlength = height-54;
			
			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),height,6,1,0);
			draw_rect(button_x(buttonx),button_y(buttony),92,height,1,155,0,0);

			float fvalue = 0;

			if(dmx_img_dim_get(stash[act],&fvalue))
			{
				unsigned int position = runlength * (1-fvalue);
				draw_filledRect(button_x(buttonx)+1,button_y(buttony)+position+1,92-2,54-2,155,100,0);
			}
			buttonx++;

			touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,6,2,0);
			draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Clear",155,0,0,0,255,0);

		}
	}
	else if(tab==5)
	{
		draw_number_8x6(button_x(buttonx++)+20,button_y(buttony)+20, act_wait_value, 7, '_', 255,255,255);
		buttony++;buttonx=0;
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,7);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"7",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,8);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"8",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,9);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"9",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,3,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Millisecond",155,(act_wait_type==0)?155:0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,2,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"<--",155,0,0,0,255,0);
		buttony++;buttonx=0;
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,4);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"4",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,5);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"5",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,6);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"6",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,3,1);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"Milliibeats",155,(act_wait_type==1)?155:0,0,0,255,0);
		buttony++;buttonx=0;
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,1);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"1",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,2);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"2",155,0,0,0,255,0);
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,3);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"3",155,0,0,0,255,0);
		buttony++;buttonx=0;
		touch_binding_add(touchlist,button_x(buttonx),92,button_y(buttony),54,13,1,0);
		draw_button_icon(button_x(buttonx++),button_y(buttony),92,1,"0",155,0,0,0,255,0);
		buttony++;buttonx=0;
	}


		
	
	if(list1 != NULL)
	{
		touch_binding_add(touchlist,button_x(5),297,button_y(3),listsize*18-4,7,0,0);
		menu_list_draw(list1,button_x(5),button_y(3),listsize);
	}

}

static void return_from_store(char * name)
{
	strncpy(prog_stack->name,name,DMX_NAME_LENGTH);
	dmx_stack_store(prog_stack);
	dmx_stack_store_to_disc("stack");
}


static void update_list(void)
{
			unsigned int selected = 0;
			if(list1 != NULL)
			{
				selected = list1->length;
				menu_list_free(list1);
			}
			list1 = menu_list_new();
			for(unsigned int i=0;i < dmx_stack_frame_count(prog_stack);i++)
			{
				dmx_frame* frame = dmx_stack_frame_getbyidx(prog_stack,i);

				char* label = malloc(sizeof(char)*DMX_NAME_LENGTH);

				if(frame->type == DMX_FRAME_IMAGE)
				{
					struct dmx_img* image = frame->image.image;
					snprintf(label,DMX_NAME_LENGTH,"IMG %i %c %s %s",
						image->dev_count,
						(image->is_dim)?'D':'-',
						(image->is_col==DMX_ATTR_COLOR_NAME)?"CN":"--",
						(image->is_col==DMX_ATTR_COLOR_RGB)?"CRGB":"----");
				}
				else if(frame->type == DMX_FRAME_WAIT)
				{
					if(frame->wait.milis>0)
					{
						snprintf(label,DMX_NAME_LENGTH,"WAIT %i ms",frame->wait.milis);
					}
					else
					{
						snprintf(label,DMX_NAME_LENGTH,"WAIT %i mbpm",frame->wait.bpms);
					}
				}
				else
				{
					snprintf(label,DMX_NAME_LENGTH,"?");
				}

				menu_list_add_entry(list1, menu_list_entry_new(MENU_LIST_ENTRY_LABEL,label,0,0),-1);
				free(label);
			}


			unsigned int offset = 0;
			if(selected > (listsize-1))
			{
				offset = selected-(listsize-1);
				selected = selected - offset;
			}
			list1->selected=selected;
			list1->offset=offset;
}


static void menu_prog_touch(unsigned int x, unsigned int y)
{
	if(y < 41)
	{
		if(x < 40)
		{
			if(menu_prog->parent != NULL)
				set_current_menu(menu_prog->parent);
		}
	}

	unsigned int attr1 = 0;
	unsigned int attr2 = 0;
	unsigned int attr3 = 0;
	unsigned int relx = 0;
	unsigned int rely = 0;
	if(touch_test(touchlist,x,y,&attr1,&attr2,&attr3,&relx,&rely))
	{
		if(attr1 == 1)
		{
			tab = (tab !=attr2)?attr2:0;
			set_menu_dirty();
		}
		else if(attr1 == 2)
		{
			if(act_frame_type == DMX_FRAME_IMAGE)
			{
				struct dmx_img* img = dmx_img_clone(stash[act]);
				dmx_stack_add_imgframe(prog_stack,img);
		
			}
			else if(act_frame_type == DMX_FRAME_WAIT)
			{
				dmx_stack_add_waitframe(prog_stack,act_wait_type,act_wait_value);
			}

			update_list();
			set_menu_dirty();
		}
		else if(attr1 == 11) // store
		{
			invoke_keyboard("Store Programmer List",prog_stack->name,return_from_store);
		}
		else if(attr1 == 12) // set frame type
		{
			act_frame_type = attr2;
			if(act_frame_type==DMX_FRAME_WAIT)
			{
				tab=5;
			}
			set_menu_dirty();
		}
		else if(attr1 == 13) // set wait value
		{
			if((attr2 == 1)&&(act_wait_value < 1000000))
			{
				act_wait_value*=10;
				act_wait_value+=attr3;
			}
			else if((attr2 == 2)&&(act_wait_value > 0))
			{
				act_wait_value-=act_wait_value%10;
				act_wait_value/=10;
			}
			else if(attr2 == 3)
			{
				act_wait_type=attr3;
			}
			set_menu_dirty();
		}
		else if(attr1 == 10) // set active attribute
		{
			if(act_attribute == attr2)
			{
				act_attribute = 0;
			}
			else
			{
				act_attribute = attr2;
				tab=4;
			}
			set_menu_dirty();
		}
		else if(attr1 == 8) // replace selected list entry 
		{
			if((list1 != NULL)&&(list1->length>0))
			{
				//signed int selected = menu_list_get_selected(list1);
				
				//todo: implement it

				set_menu_dirty();
			}
		}
		else if(attr1 == 14) // preview loop
		{
			if((list1 != NULL)&&(list1->length>0))
			{
				if(loop_enable==0)
				{
					loop_enable=1;
					set_programmer_loop(1);
				}
				else
				{
					loop_enable=0;
					set_programmer_loop(0);
				}

				set_menu_dirty();
			}
		}
		else if(attr1 == 9) // delete selected list entry 
		{
			if((list1 != NULL)&&(list1->length>0))
			{
				signed int selected = menu_list_get_selected(list1);
				dmx_stack_del_imgframe(prog_stack,selected);
				menu_list_del_entry_by_idx(list1, selected);
				set_menu_dirty();
			}
		}
		else if(attr1 == 3)
		{
			struct dmx_device* dmx_device = dmx_get_device_byidx(attr2);

			if(dmx_img_device_test(stash[act],dmx_device->name))
			{
				dmx_img_device_del(stash[act],dmx_device->name);
			}
			else
			{
				dmx_img_device_add(stash[act],dmx_device->name);
			}

			set_menu_dirty();
		}
		else if(attr1 == 4)
		{
			const char * colorname = dmx_attr_colors_get_name(attr2);
			if(dmx_img_color_test(stash[act],colorname) != 1)
			{
				dmx_img_color_clear(stash[act]);
				dmx_img_color_setname(stash[act],colorname);
			}
			else
			{
				dmx_img_color_clear(stash[act]);
			}
			set_menu_dirty();
		}
		else if(attr1 == 5)
		{
			unsigned int r=0;
			unsigned int g=0;
			unsigned int b=0;
			dmx_img_color_getrgb(stash[act],&r,&g,&b);

			if(rely > (54/2))
			{
				rely -= (54/2);
			}
			else
			{
				rely=0;
			}
			unsigned int height = button_y(5)-button_y(0)-11;
			unsigned int runlength = height-54;

			if(attr2 == 1)
			{
				r = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 2)
			{
				g = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 3)
			{
				b = 255-(rely/(float)runlength*255.0f);
				dmx_img_color_setrgb(stash[act],r,g,b);
			}	
			else if(attr2 == 4)
			{
				dmx_img_color_clear(stash[act]);
			}
			set_menu_dirty();
		}
		else if(attr1 == 6)
		{
			if(attr2 == 1)
			{
				if(rely > (54/2))
				{
					rely -= (54/2);
				}
				else
				{
					rely=0;
				}
				unsigned int height = button_y(5)-button_y(0)-11;
				unsigned int runlength = height-54;

				dmx_img_dim_set(stash[act],1-(rely/(float)runlength));
			}
			else if(attr2 == 2)
			{
				dmx_img_dim_clear(stash[act]);
			}
			set_menu_dirty();
		}
		else if(attr1 == 7)
		{
			signed int selected = menu_list_touch(list1,relx,rely);
			if(selected >= 0)
			{
				dmx_frame* frame = prog_stack->frames[selected];
				
				if(frame->type == DMX_FRAME_IMAGE)
				{
					act_frame_type = DMX_FRAME_IMAGE;
					if(tab==5)
					{
						tab=3;
					}

					dmx_img_free(stash[act]);
					stash[act]=dmx_img_clone(frame->image.image);
				}
				else if(frame->type == DMX_FRAME_WAIT)
				{
					act_frame_type = DMX_FRAME_WAIT;
					tab=5;
					
					if(frame->wait.milis>0)
					{
						act_wait_type=0;
						act_wait_value=frame->wait.milis;
					}
					else
					{
						act_wait_type=1;
						act_wait_value=frame->wait.bpms;
					}

				}
				
				set_menu_dirty();
			}
		}
	}
}

void menu_prog_load_stack(struct dmx_stack* stack)
{
	if(prog_stack != NULL)
	{
		set_programmer_stack(NULL);
		dmx_stack_free(prog_stack);
	}

	prog_stack=dmx_stack_clone(stack);
	prog_stack->active=1;
	set_programmer_stack(prog_stack);
	printf("loaded %s %d\n",stack->name,stack->length);
			
	update_list();
	
	set_menu_dirty();
}

struct menu* get_menu_prog(void)
{
	if(menu_prog == NULL)
	{
		menu_prog = malloc(sizeof(struct menu));
		menu_prog->redraw = menu_prog_redraw;
		menu_prog->touch = menu_prog_touch;
		menu_prog->parent = NULL;

		stash[0] = dmx_img_new();
		stash[1] = dmx_img_new();
		stash[2] = dmx_img_new();
		stash[3] = dmx_img_new();
		set_programmer_image_list(&stash[0]);
	}
	if(prog_stack == NULL)
	{
		prog_stack= dmx_stack_new();

		prog_stack->active=1;
		set_programmer_stack(prog_stack);
	}
	return menu_prog;
}

