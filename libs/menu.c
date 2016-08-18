#include <stdlib.h>
#include <stdint.h>

#include "pixel_hal.h"
#include "menu.h"
#include "sdl_util.h"


static unsigned int* current_pixelbuffer=NULL;

void clearDisplay()
{
	int x, y;
	for(x = 0; x < LCD_WIDTH; x++) {
		for(y = 0; y < LCD_HEIGHT; y++) {
			current_pixelbuffer[(y*LCD_WIDTH)+x]=0;
		}
	}
}

void setLedXY(uint16_t x, uint16_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;

	if(current_pixelbuffer != NULL)
	{
		unsigned int color = (red<<16)+(green<<8)+blue;
		current_pixelbuffer[(y*LCD_WIDTH)+x] = color;
	}
}

void invLedXY(uint16_t x, uint16_t y) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
//	leds[y][x][0] = 255 - leds[y][x][0];
//	leds[y][x][1] = 255 - leds[y][x][1];
//	leds[y][x][2] = 255 - leds[y][x][2];
//	leds[y][x][3] = 1;
}

void getLedXY(uint16_t x, uint16_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;

	if(current_pixelbuffer != NULL)
	{
		unsigned int color = current_pixelbuffer[(y*LCD_WIDTH)+x];
		*red = (color>>16);
		*green = (color>>8) & 0xff;
		*blue = color & 0xff;
	}
}


#include "gui/menu_main.h"

static struct menu* current_menu;

static unsigned int dirty = 1;
static unsigned int menuauto = 0;

void menu_autoupdate()
{
	menuauto = 1;
};

struct menu* get_current_menu(void)
{
	return current_menu;
}

void set_current_menu(struct menu* new_menu)
{
	current_menu = new_menu;
	dirty = 1;
	menuauto = 0;
}
void set_menu_dirty(void)
{
	dirty = 1;
}

void menu_init()
{
	current_menu = get_menu_main();
	dirty = 1;
	menuauto = 0;
}
void draw_menu(unsigned int* pixelbuffer)
{
	unsigned int x;
	unsigned int y;

	if(get_touch(&x,&y)==1)
	{
		current_menu->touch(x,y);
	}

	if(dirty || menuauto)
	{
		current_pixelbuffer=pixelbuffer;
		if(menuauto==1)
			if(current_menu->update != NULL)
				current_menu->update();
		if(dirty==1)
			if(current_menu->redraw != NULL)
				current_menu->redraw();
		current_pixelbuffer=NULL;
		dirty = 0;
	}
}
		
unsigned int button_x(unsigned int x)
{
	return x*103+11;
}
unsigned int button_y(unsigned int y)
{
	return y*65+45;
}

