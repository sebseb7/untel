#include <stdlib.h>


#include "menu.h"
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

static struct menu* menu_quectrl = NULL;

static void menu_quectrl_redraw(void)
{
		clearDisplay();
		//clear_buttons();

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);
	
		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("CUE CTRL");
		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "CUE CTRL", 200,200,255);

		for(unsigned int i =0;i< dmx_queue_get_count();i++)
		{
			struct dmx_queue* dmx_queue = dmx_queue_getbyidx(i);
			draw_button_icon(button_x(0),button_y(i),92,1,dmx_queue->name,155,0,0,0,255,0);
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

}

struct menu* get_menu_quectrl()
{
	if(menu_quectrl == NULL)
	{
		menu_quectrl = malloc(sizeof(struct menu*));
		menu_quectrl->redraw = menu_quectrl_redraw;
		menu_quectrl->touch = menu_quectrl_touch;
		menu_quectrl->parent = NULL;
	}
	return menu_quectrl;
}

