#ifndef _OSC_H
#define _OSC_H

#include <stdint.h>

void osc_process_input(void);
void osc_init(void);
void osc_deinit(void);
void osc_apply_manual(unsigned char* out);
void osc_update_ui(void);

#endif
