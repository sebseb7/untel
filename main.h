#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

//#define OSC_OUT 1
//#define CONCH_OUT 1
#include "dmx_frame.h"

void set_programmer_image_list(struct dmx_img* stash);


#endif

