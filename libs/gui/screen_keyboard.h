#ifndef _SCREEN_KEYBOARD_H
#define _SCREEN_KEYBOARD_H

#include <stdint.h>

void invoke_keyboard(char*,char*,void (*)(char*));

void invoke_numeric_keyboard(char*,uint32_t,void (*)(uint32_t));
void invoke_addr_keyboard(char*,uint32_t,void (*)(uint32_t));

#endif

