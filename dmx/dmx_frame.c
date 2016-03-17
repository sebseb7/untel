#include <stdlib.h>

#include "dmx_frame.h"


dmx_frame* dmx_frame_new(unsigned int type)
{
	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type = type;

	if(type == DMX_FRAME_IMAGE)
	{
		struct dmx_img* image = malloc(sizeof(struct dmx_img));

		image->dev_alloc=0;
		image->dev_count=0;
		image->dev_names=NULL;

		image->is_dim=0;
		image->is_col=0;
		image->dim=0.0f;
		image->r=0;
		image->g=0;
		image->b=0;
		image->color=NULL;
		frame->image.image = image;
	}
	else if(type == DMX_FRAME_WAIT)
	{
		frame->wait.milis = 0;
	}
	else if(type == DMX_FRAME_COMMAND)
	{
		frame->command.command = 0;
		frame->command.svalue1 = NULL;
		frame->command.svalue2 = NULL;
		frame->command.ivalue1=0;
		frame->command.ivalue2=0;
		frame->command.fvalue1=0.0f;
		frame->command.fvalue2=0.0f;
	}
	
	return frame;
}

