#include <stdint.h>

#include "pixel_hal.h"
#include "menu.h"


static uint8_t leds[LCD_HEIGHT][LCD_WIDTH][4];

void clearDisplay()
{
	int x, y;
	for(x = 0; x < LCD_WIDTH; x++) {
		for(y = 0; y < LCD_HEIGHT; y++) {
			leds[y][x][0] = 0;
			leds[y][x][1] = 0;
			leds[y][x][2] = 0;
			leds[y][x][3] = 1;
		}
	}
}

void setLedXY(uint16_t x, uint16_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
	leds[y][x][3] = 1;
}

void invLedXY(uint16_t x, uint16_t y) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	leds[y][x][0] = 255 - leds[y][x][0];
	leds[y][x][1] = 255 - leds[y][x][1];
	leds[y][x][2] = 255 - leds[y][x][2];
	leds[y][x][3] = 1;
}
void getLedXY(uint16_t x, uint16_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
}


#include "gui/menu_main.h"

static void (*current_execution)(void);
void set_current_execution(void (*new_execution)(void))
{
	current_execution = new_execution;
}
void (*get_current_execution(void))(void)
{
	return current_execution;
}
void menu_init()
{
	current_execution = menu_main;
}
void draw_menu()
{
	current_execution();
}
		
unsigned int button_x(unsigned int x)
{
	return x*103+11;
}
unsigned int button_y(unsigned int y)
{
	return y*65+45;
}

