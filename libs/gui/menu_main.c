#include <stdlib.h>


#include "menu.h"

#include "menu_main.h"
#include "menu_prog.h"
#include "menu_setup.h"
#include "menu_quectrl.h"
//#include "menu_directdmx.h"
//#include "screen_keyboard.h"
//#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"

static struct menu* menu_main = NULL;

#include "touch_binding.h"


static struct touch_binding_list* touchlist = NULL;

static void menu_main_redraw(void)
{
	clearDisplay();
	//clear_buttons();
	if(touchlist != NULL)
		touch_binding_free(touchlist);
	touchlist = touch_binding_new();

	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	uint16_t text_width =  get_text_width_16pt("DMX BOX");
	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "DMX BOX", 200,200,255);


	touch_binding_add(touchlist,button_x(0),92,button_y(0),54,1,0,0);
	draw_button_icon(button_x(0),button_y(0),92,1,"Cue Ctrl",155,0,0,0,255,0);
	draw_button_icon(button_x(1),button_y(0),92,1,"",55,55,55,0,0,0);
	touch_binding_add(touchlist,button_x(2),92,button_y(0),54,2,0,0);
	draw_button_icon(button_x(2),button_y(0),92,1,"Prog",155,0,0,0,255,0);
	draw_button_icon(button_x(0),button_y(1),92,1,"",55,55,55,0,0,0);
	draw_button_icon(button_x(1),button_y(1),92,1,"",55,55,55,0,0,0);
	draw_button_icon(button_x(2),button_y(1),92,1,"",55,55,55,0,0,0);
	draw_button_icon(button_x(0),button_y(2),92,1,"Direct DMX",155,0,0,0,255,0);
	draw_button_icon(button_x(1),button_y(2),92,1,"",55,55,55,0,0,0);
	touch_binding_add(touchlist,button_x(2),92,button_y(2),54,3,0,0);
	draw_button_icon(button_x(2),button_y(2),92,1,"Setup",155,0,0,0,255,0);

}


static void menu_main_touch(unsigned int x, unsigned int y)
{
	unsigned int attr1 = 0;
	unsigned int attr2 = 0;
	unsigned int attr3 = 0;
	unsigned int relx = 0;
	unsigned int rely = 0;
	if(touch_test(touchlist,x,y,&attr1,&attr2,&attr3,&relx,&rely))
	{

		if(attr1 == 1)
		{
			struct menu* menu_quectrl = get_menu_quectrl();
			menu_quectrl->parent=menu_main;
			set_current_menu(menu_quectrl);
		}
		else if(attr1 == 2)
		{
			struct menu* menu_prog = get_menu_prog();
			menu_prog->parent=menu_main;
			set_current_menu(menu_prog);
		}
		else if(attr1 == 3)
		{
			struct menu* menu_setup = get_menu_setup();
			menu_setup->parent=menu_main;
			set_current_menu(menu_setup);
		}
	}
}

struct menu* get_menu_main()
{
	if(menu_main == NULL)
	{
		menu_main = malloc(sizeof(struct menu));
		menu_main->redraw = menu_main_redraw;
		menu_main->touch = menu_main_touch;
		menu_main->parent = NULL;
	}

	return menu_main;
}

