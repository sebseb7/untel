#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "menu_prog.h"
#include "menu_main.h"
#include "menu_quectrl.h"
#include "menu_setup.h"
//#include "menu_directdmx.h"
//#include "screen_keyboard.h"
//#include "dmxbox_hal.h"
#include "mcugui/circle.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"

#include "dmx_queue.h"
#if LUA_CUE==1
#include "dmx_luaqueue.h"
#endif
#include "dmx_stack.h"

static struct menu* menu_quectrl = NULL;

#include "touch_binding.h"
static struct touch_binding_list* touchlist = NULL;

static unsigned int mode = 0;

static void menu_quectrl_redraw(void)
{
	clearDisplay();
	//clear_buttons();
	if(touchlist != NULL)
		touch_binding_free(touchlist);
	touchlist = touch_binding_new();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	draw_filledCircle(17,17,15,40,80,40);
	draw_text_8x6(7,10,"Back",255,100,100);

	uint16_t text_width =  get_text_width_16pt("CUE CTRL");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "CUE CTRL", 200,200,255);

	unsigned int x=0;
	unsigned int y=0;
	for(unsigned int i =0;i< dmx_queue_get_count();i++)
	{
		struct dmx_queue* dmx_queue = dmx_queue_getbyidx(i);
		touch_binding_add(touchlist,button_x(x),92,button_y(y),54,1,i,0);
		draw_button_icon(button_x(x),button_y(y),92,1,dmx_queue->name,155,0,0,0,255,0);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
	x=0;y++;
#if LUA_CUE==1
	for(unsigned int i =0;i< dmx_luaqueue_get_count();i++)
	{
		struct dmx_luaqueue* dmx_luaqueue = dmx_luaqueue_getbyidx(i);
		touch_binding_add(touchlist,button_x(x),92,button_y(y),54,2,i,0);
		draw_button_icon(button_x(x),button_y(y),92,1,dmx_luaqueue->name,155,0,0,0,255,0);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
	if(dmx_luaqueue_get_count())
	{
		x=0;
		y++;
	}
#endif
			
	touch_binding_add(touchlist,button_x(x),92,button_y(y),54,4,1,0);
	draw_button_icon(button_x(x++),button_y(y),92,1,"Load",155,(mode==1)?155:0,0,0,255,0);
	touch_binding_add(touchlist,button_x(x),92,button_y(y),54,4,2,0);
	draw_button_icon(button_x(x++),button_y(y),92,1,"Delete",155,(mode==2)?155:0,0,0,255,0);
	x=0;
	y++;
	
	for(unsigned int i =0;i< dmx_stack_get_count();i++)
	{
		struct dmx_stack* stack = dmx_stack_getbyidx(i);
		touch_binding_add(touchlist,button_x(x),92,button_y(y),54,3,i,0);
		draw_button_icon(button_x(x),button_y(y),92,1,stack->name,155,0,0,0,255,0);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
	menu_autoupdate();
}

static void menu_quectrl_update(void)
{
	draw_filledRect(0,37,1024,500,0,0,0);
	unsigned int x=0;
	unsigned int y=0;
	for(unsigned int i =0;i< dmx_queue_get_count();i++)
	{
		struct dmx_queue* dmx_queue = dmx_queue_getbyidx(i);
		draw_button_icon(button_x(x),button_y(y),92,1,dmx_queue->name,155,(dmx_queue->active)?155:0,0,0,255,0);
		//draw_number_8x6(button_x(x),button_y(y),dmx_queue->active,2,0,255,255,255);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
	x=0;y++;
#if LUA_CUE==1
	for(unsigned int i =0;i< dmx_luaqueue_get_count();i++)
	{
		struct dmx_luaqueue* dmx_luaqueue = dmx_luaqueue_getbyidx(i);
		draw_button_icon(button_x(x),button_y(y),92,1,dmx_luaqueue->name,155,(dmx_luaqueue->active_cnt)?155:0,0,0,255,0);
		//draw_number_8x6(button_x(x),button_y(y),dmx_luaqueue->active_cnt,2,0,255,255,255);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
#endif
	
	draw_button_icon(button_x(x++),button_y(y),92,1,"Load",155,(mode==1)?155:0,0,0,255,0);
	draw_button_icon(button_x(x++),button_y(y),92,1,"Delete",155,(mode==2)?155:0,0,0,255,0);
	x=0;
	y++;
	
	for(unsigned int i =0;i< dmx_stack_get_count();i++)
	{
		struct dmx_stack* stack = dmx_stack_getbyidx(i);
		draw_button_icon(button_x(x),button_y(y),92,1,stack->name,155,(stack->active)?155:0,0,0,255,0);
		//draw_number_8x6(button_x(x),button_y(y),dmx_luaqueue->active_cnt,2,0,255,255,255);
		x++;
		if(x>7)
		{
			x=0;
			y++;
		}
	}
}


static void menu_quectrl_touch(unsigned int x, unsigned int y)
{
	if(y < 41)
	{
		if(x < 40)
		{
			if(menu_quectrl->parent != NULL)
				set_current_menu(menu_quectrl->parent);
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
			struct dmx_queue* dmx_queue = dmx_queue_getbyidx(attr2);
			if(dmx_queue->active)
			{
				dmx_queue_deactivate(dmx_queue);
			}
			else
			{
				dmx_queue_activate(dmx_queue);
			}
		}
#if LUA_CUE==1
		else if(attr1 == 2)
		{
			struct dmx_luaqueue* dmx_queue = dmx_luaqueue_getbyidx(attr2);
			if(dmx_queue->active_cnt)
			{
				dmx_luaqueue_deactivate(dmx_queue);
			}
			else
			{
				dmx_luaqueue_activate(dmx_queue);
			}
		}
#endif
		else if(attr1 == 3)
		{
			if(mode == 1)//load
			{
				mode = 0;
				struct dmx_stack* stack = dmx_stack_getbyidx(attr2);
				menu_prog_load_stack(stack);
				printf("laod %s\n",stack->name);

				struct menu* menu_prog = get_menu_prog();
				menu_prog->parent=menu_quectrl;
				set_current_menu(menu_prog);
			}
			else if(mode == 2)//delete
			{
				mode = 0;
				dmx_stack_delete(attr2);
			}
			else//toggle
			{
				struct dmx_stack* stack = dmx_stack_getbyidx(attr2);
				if(stack->active == 0 )
				{
					if(strnlen(stack->group,DMX_NAME_LENGTH) > 0)
					{
						struct dmx_stack* stack2 = dmx_stack_getactiveingroup(stack->group);
						if(stack2!=NULL)
						{
							printf("radio !\n");
							stack2->active=0;
						}


						printf("group\n");
					};	
				
					stack->active = 1;
				}
				else
				{
					stack->active = 0;
				}
			}
		}
		else if(attr1 == 4)
		{
			if(mode == attr2)
			{
				mode = 0;
			}
			else
			{
				mode = attr2;
			}
		}
	}

}

struct menu* get_menu_quectrl()
{
	if(menu_quectrl == NULL)
	{
		menu_quectrl = malloc(sizeof(struct menu));
		menu_quectrl->update = menu_quectrl_update;
		menu_quectrl->redraw = menu_quectrl_redraw;
		menu_quectrl->touch = menu_quectrl_touch;
		menu_quectrl->parent = NULL;
	}
	return menu_quectrl;
}

