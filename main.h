#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

//#define OSC_OUT 1
#define SDL_OUT 1
#if FTDI_AVAIL==1 
//#define DMX_OUT 1
#endif
//#define CONCH_OUT 1


#endif

