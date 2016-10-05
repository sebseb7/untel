#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmx_stack.h"

#include "main.h"

#include "SDL.h"

#define DMX_STACK_FRAMES_ALLOCATE_INITIAL 100
#define DMX_STACK_FRAMES_ALLOCATE_STEP 10
#define DMX_STACK_ALLOCATE_INITIAL 100
#define DMX_STACK_ALLOCATE_STEP 10

static struct dmx_stack** dmx_stack_list;

static unsigned int dmx_stack_allocated=0;
static unsigned int dmx_stack_inuse=0;

struct dmx_stack* dmx_stack_new(void)
{
	struct dmx_stack* stack = malloc(sizeof(struct dmx_stack));

	strcpy(stack->name,"");
	strcpy(stack->group,"");
	stack->active=0;
	stack->lastframetime=0;
	stack->blendpointer=0;
	stack->length=0;
	stack->alloc=DMX_STACK_FRAMES_ALLOCATE_INITIAL;
	stack->frames=malloc(sizeof(dmx_frame*)*DMX_STACK_FRAMES_ALLOCATE_INITIAL);

	return stack;
}

unsigned int dmx_stack_get_count(void)
{
	return dmx_stack_inuse;

}
struct dmx_stack* dmx_stack_getbyidx(unsigned int index)
{
	if(index < dmx_stack_inuse)
	{
		return dmx_stack_list[index];
	}
	return NULL;
}

void dmx_stack_delete(unsigned int index)
{
	if(index >= dmx_stack_inuse)
		return;

	dmx_stack_free(dmx_stack_list[index]);

	for(unsigned int x = index;x < dmx_stack_inuse;x++)
	{
		dmx_stack_list[x]=dmx_stack_list[x+1];
	}
	dmx_stack_inuse--;
}

struct dmx_stack* dmx_stack_getbyname(char* name)
{	
	for(unsigned int i=0;i<dmx_stack_inuse;i++)
	{
		if(0==strncmp(dmx_stack_list[i]->name,name,DMX_NAME_LENGTH))
		{
			return dmx_stack_list[i];
		}
	}
	return NULL;
}
struct dmx_stack* dmx_stack_getactiveingroup(char* group)
{	
	for(unsigned int i=0;i<dmx_stack_inuse;i++)
	{
		if(0==strncmp(dmx_stack_list[i]->group,group,DMX_NAME_LENGTH))
		{
			if(dmx_stack_list[i]->active != 0)
				return dmx_stack_list[i];
		}
	}
	return NULL;
}

void dmx_frame_free(dmx_frame* frame)
{
	if(frame->type == DMX_FRAME_IMAGE)
	{
		dmx_img_free(frame->image.image);
	}
	if(frame->type == DMX_FRAME_COMMAND)
	{
		if(frame->command.svalue1 != NULL)
			free(frame->command.svalue1);
		if(frame->command.svalue2 != NULL)
			free(frame->command.svalue2);
	}
	free(frame);
}


void dmx_stack_free(struct dmx_stack* stack)
{
	for (unsigned int c = 0 ; c < stack->length; c++ )
	{
		dmx_frame_free(stack->frames[c]);
	}
	free(stack->frames);
	free(stack);
}

void dmx_stack_store(struct dmx_stack* stack)
{

	for(unsigned int i=0;i<dmx_stack_inuse;i++)
	{
		if(0==strncmp(dmx_stack_list[i]->name,stack->name,DMX_NAME_LENGTH))
		{
			dmx_stack_free(dmx_stack_list[i]);
			dmx_stack_list[i]=dmx_stack_clone(stack);
			return;
		}
	}

	if(0==dmx_stack_allocated)
	{
		dmx_stack_list = malloc(sizeof(*dmx_stack_list)*DMX_STACK_ALLOCATE_INITIAL);
		dmx_stack_allocated=DMX_STACK_ALLOCATE_INITIAL;
	}

	if(dmx_stack_inuse == dmx_stack_allocated)
	{
		printf("stf21\n");
		exit(EXIT_FAILURE);
	}

	dmx_stack_list[dmx_stack_inuse]=dmx_stack_clone(stack);
	dmx_stack_inuse++;

}

void dmx_stack_store_to_disc(char * file_name)
{
	//	char *file_name = "stack";
	char *base_path = SDL_GetPrefPath("net.exse", "untel");
	char *file_path = calloc((strlen(base_path)+strlen(file_name)+1),sizeof(char));
	strcat(file_path,base_path);
	strcat(file_path,file_name);
	free(base_path);	
	SDL_RWops *file = SDL_RWFromFile(file_path, "w");
	free(file_path);

	SDL_WriteBE32(file,dmx_stack_inuse);
	for(unsigned int i=0;i<dmx_stack_inuse;i++)
	{
		SDL_WriteU8(file,strlen(dmx_stack_list[i]->name));
		SDL_RWwrite(file,dmx_stack_list[i]->name,1,1+strlen(dmx_stack_list[i]->name));
		SDL_WriteU8(file,strlen(dmx_stack_list[i]->group));
		SDL_RWwrite(file,dmx_stack_list[i]->group,1,1+strlen(dmx_stack_list[i]->group));
		SDL_WriteBE32(file,dmx_stack_list[i]->length);

		for(unsigned int c=0;c<dmx_stack_list[i]->length;c++)
		{
			SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->type);

			if(dmx_stack_list[i]->frames[c]->type == DMX_FRAME_IMAGE)
			{

				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->dev_count);
				for(unsigned int x=0;x<dmx_stack_list[i]->frames[c]->image.image->dev_count;x++)
				{
					SDL_WriteU8(file,strlen(dmx_stack_list[i]->frames[c]->image.image->dev_names[x]));
					SDL_RWwrite(file,dmx_stack_list[i]->frames[c]->image.image->dev_names[x],1,1+strlen(dmx_stack_list[i]->frames[c]->image.image->dev_names[x]));
				}
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->is_dim);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->is_col);

				char output[50];
				snprintf(output, 50, "%f", dmx_stack_list[i]->frames[c]->image.image->dim);

				SDL_WriteU8(file,strlen(output));
				SDL_RWwrite(file,output,1,1+strlen(output));

				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->r);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->g);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->image.image->b);

				if(dmx_stack_list[i]->frames[c]->image.image->color)
				{
					SDL_WriteU8(file,strlen(dmx_stack_list[i]->frames[c]->image.image->color));
					SDL_RWwrite(file,dmx_stack_list[i]->frames[c]->image.image->color,1,1+strlen(dmx_stack_list[i]->frames[c]->image.image->color));
				}
				else
				{
					SDL_WriteU8(file,0);
				}

			}
			else if(dmx_stack_list[i]->frames[c]->type == DMX_FRAME_WAIT)
			{

				SDL_WriteU8(file,dmx_stack_list[i]->frames[c]->wait.blend);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->wait.milis);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->wait.bpms);

			}
			else if(dmx_stack_list[i]->frames[c]->type == DMX_FRAME_COMMAND)
			{
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->command.command);

				if(dmx_stack_list[i]->frames[c]->command.svalue1)
				{
					SDL_WriteU8(file,strlen(dmx_stack_list[i]->frames[c]->command.svalue1));
					SDL_RWwrite(file,dmx_stack_list[i]->frames[c]->command.svalue1,1,1+strlen(dmx_stack_list[i]->frames[c]->command.svalue1));
				}
				else
				{
					SDL_WriteU8(file,0);
				}
				if(dmx_stack_list[i]->frames[c]->command.svalue2)
				{
					SDL_WriteU8(file,strlen(dmx_stack_list[i]->frames[c]->command.svalue2));
					SDL_RWwrite(file,dmx_stack_list[i]->frames[c]->command.svalue2,1,1+strlen(dmx_stack_list[i]->frames[c]->command.svalue2));
				}
				else
				{
					SDL_WriteU8(file,0);
				}


				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->command.ivalue1);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->command.ivalue2);

				char output[50];

				snprintf(output, 50, "%f", dmx_stack_list[i]->frames[c]->command.fvalue1);
				SDL_WriteU8(file,strlen(output));
				SDL_RWwrite(file,output,1,1+strlen(output));

				snprintf(output, 50, "%f", dmx_stack_list[i]->frames[c]->command.fvalue2);
				SDL_WriteU8(file,strlen(output));
				SDL_RWwrite(file,output,1,1+strlen(output));

			}
			else
			{
				printf("cannot save frame\n");
			}

		}
	}

	//frames need to be stored as well

	SDL_RWclose(file);
}

void dmx_stack_load_from_disc(void)
{
	char *file_name = "stack";
	char *base_path = SDL_GetPrefPath("net.exse", "untel");
	char *file_path = calloc((strlen(base_path)+strlen(file_name)+1),sizeof(char));
	strcat(file_path,base_path);
	strcat(file_path,file_name);
	free(base_path);	
	//printf("file: %s\n",file_path);
	SDL_RWops *file = SDL_RWFromFile(file_path, "r");
	free(file_path);

	if(file == NULL)
		return;


	unsigned int stacks = SDL_ReadBE32(file);


	//printf("stacks: %i\n",stacks);

	for(unsigned int x = 0; x < stacks;x++)
	{
		struct dmx_stack* stack = malloc(sizeof(struct dmx_stack));

		//printf("stack:\n");

		unsigned int name_length = SDL_ReadU8(file);
		if(name_length > (DMX_NAME_LENGTH-1)) name_length=DMX_NAME_LENGTH-1;
		SDL_RWread(file, stack->name, 1, name_length+1);
		unsigned int group_length = SDL_ReadU8(file);
		if(group_length > (DMX_NAME_LENGTH-1)) group_length=DMX_NAME_LENGTH-1;
		SDL_RWread(file, stack->group, 1, group_length+1);
		stack->active=0;
		stack->length=0;

		stack->alloc=SDL_ReadBE32(file);// if we define a limit later, enforce it here
		stack->frames=malloc(sizeof(dmx_frame*)*stack->alloc);

		//printf("alloc: %i\n",stack->alloc);

		for(unsigned int c=0;c<stack->alloc;c++)
		{
			unsigned int frame_type = SDL_ReadBE32(file);

			//printf("ft: %i\n",frame_type);

			if(frame_type == DMX_FRAME_IMAGE)
			{

				struct dmx_img* image = dmx_img_new();

				unsigned int dev_count = SDL_ReadBE32(file);

				//printf("dev: %i\n",dev_count);// if we define a limit later, enforce it here

				for(unsigned int x = 0;x<dev_count;x++)
				{
					unsigned int dev_name_length = SDL_ReadU8(file);
					//printf("devnl: %i\n",dev_name_length);
					char input[DMX_NAME_LENGTH];
					if(dev_name_length > (DMX_NAME_LENGTH-1)) dev_name_length=DMX_NAME_LENGTH-1;
					SDL_RWread(file, input, 1, dev_name_length+1);
					dmx_img_device_add(image, input);
				}

				image->is_dim = SDL_ReadBE32(file);//todo: sanity check
				image->is_col = SDL_ReadBE32(file);//todo: sanity check

				unsigned int float_length = SDL_ReadU8(file);
				//printf("float nl: %i\n",float_length);
				char floatinput[50];
				if(float_length > 49) float_length=49;
				SDL_RWread(file, floatinput, 1, float_length+1);

				image->dim = atof(floatinput);

				//printf("dim: %f\n",image->dim);

				image->r = SDL_ReadBE32(file);
				image->g = SDL_ReadBE32(file);
				image->b = SDL_ReadBE32(file);

				unsigned int col_name_length = SDL_ReadU8(file);
				//printf("color nl: %i\n",col_name_length);
				if(col_name_length > 0)
				{
					char tmp_colorname[DMX_NAME_LENGTH];
					if(col_name_length > (DMX_NAME_LENGTH-1)) col_name_length=DMX_NAME_LENGTH-1;
					SDL_RWread(file, tmp_colorname, 1, col_name_length+1);
					image->color = strndup(tmp_colorname,DMX_NAME_LENGTH);
				}
				else
				{
					image->color = NULL;
				}

				dmx_stack_add_imgframe(stack,image);

			}
			else if(frame_type == DMX_FRAME_WAIT)
			{

				unsigned int blend = SDL_ReadU8(file);
				unsigned int milis = SDL_ReadBE32(file);
				unsigned int bpms = SDL_ReadBE32(file);

				if(milis>0)
				{
					dmx_stack_add_waitframe(stack,0,milis,blend);
				}
				else
				{
					dmx_stack_add_waitframe(stack,1,bpms,blend);
				}

			}
			else if(frame_type == DMX_FRAME_COMMAND)
			{
				//frame->command.command = 
				SDL_ReadBE32(file);

				unsigned int svalue1_length = SDL_ReadU8(file);
				if(svalue1_length > 0)
				{
					char tmp_svalue1[DMX_NAME_LENGTH];
					if(svalue1_length > (DMX_NAME_LENGTH-1)) svalue1_length=DMX_NAME_LENGTH-1;
					SDL_RWread(file, tmp_svalue1, 1, svalue1_length+1);
					//=strndup(tmp_svalue1,DMX_NAME_LENGTH);
				}
				else
				{
					//= NULL;
				}
				unsigned int svalue2_length = SDL_ReadU8(file);
				if(svalue2_length > 0)
				{
					char tmp_svalue2[DMX_NAME_LENGTH];
					if(svalue2_length > (DMX_NAME_LENGTH-1)) svalue2_length=DMX_NAME_LENGTH-1;
					SDL_RWread(file, tmp_svalue2, 1, svalue2_length+1);
					//=strndup(tmp_svalue1,DMX_NAME_LENGTH);
				}
				else
				{
					//= NULL;
				}

				//ival1 = 
				SDL_ReadBE32(file);
				//ival2 = 
				SDL_ReadBE32(file);


				unsigned int float1_length = SDL_ReadU8(file);
				char float1input[50];
				if(float1_length > 49) float1_length=49;
				SDL_RWread(file, float1input, 1, float1_length+1);

				//= atof(floatinput);

				unsigned int float2_length = SDL_ReadU8(file);
				char float2input[50];
				if(float2_length > 49) float2_length=49;
				SDL_RWread(file, float2input, 1, float2_length+1);

				//= atof(floatinput);
			}
			else
			{
				printf("BROKEN: cannot load frame\n");
				SDL_RWclose(file);
				dmx_stack_free(stack);
				return;
			}
		}

		dmx_stack_store(stack);
	}

	SDL_RWclose(file);
	dmx_stack_store_to_disc("stack_regress");
}

struct dmx_stack* dmx_stack_clone(struct dmx_stack* old_stack)
{
	struct dmx_stack* stack = malloc(sizeof(struct dmx_stack));

	strcpy(stack->name,old_stack->name);
	strcpy(stack->group,old_stack->group);
	stack->active=old_stack->active;
	stack->length=old_stack->length;
	stack->alloc=old_stack->length;
	stack->frames=malloc(sizeof(dmx_frame*)*old_stack->length);

	for (unsigned int c = 0 ; c < stack->length; c++ )
	{
		dmx_frame* frame = malloc(sizeof(dmx_frame));
		memcpy(frame, old_stack->frames[c], sizeof(dmx_frame));

		if(frame->type == DMX_FRAME_IMAGE)
		{
			frame->image.image = dmx_img_clone(old_stack->frames[c]->image.image);
		}
		if(frame->type == DMX_FRAME_COMMAND)
		{
			frame->command.svalue1 = stndup(old_stack->frames[c]->command->svalue1,DMX_NAME_LENGTH);
			frame->command.svalue2 = stndup(old_stack->frames[c]->command->svalue2,DMX_NAME_LENGTH);
		}
		stack->frames[c]=frame;
	}

	return stack;
}

static void dmx_stack_realloc_frames(struct dmx_stack* stack)
{
	if(stack->length == stack->alloc)
	{
		dmx_frame** frames = malloc(sizeof(dmx_frame*)*(stack->length+DMX_STACK_FRAMES_ALLOCATE_STEP));

		for (unsigned int c = 0 ; c < stack->length; c++ )
		{
			frames[c] = stack->frames[c];
		}

		free(stack->frames);
		stack->frames=frames;
		stack->alloc=stack->length+DMX_STACK_FRAMES_ALLOCATE_STEP;

		printf("realloc\n");
	}
}

dmx_frame* dmx_frame_newwait(unsigned int type,unsigned int value,unsigned int blend)
{
	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type=DMX_FRAME_WAIT;

	frame->wait.blend=blend;

	if(type==0)
	{
		frame->wait.milis=value;
		frame->wait.bpms=0;
	}
	else
	{
		frame->wait.milis=0;
		frame->wait.bpms=value;
	}

	return frame;
}


void dmx_stack_add_waitframe(struct dmx_stack* stack,unsigned int type,unsigned int value,unsigned int blend)
{
	dmx_stack_add_waitframe_atidx(stack,type,value,blend,-1);
}

void dmx_stack_add_waitframe_atidx(struct dmx_stack* stack,unsigned int type,unsigned int value,unsigned int blend,signed int idx)
{
	if(stack->length == stack->alloc)
	{
		dmx_stack_realloc_frames(stack);
	}

	stack->length++;
	if(idx == -1)
	{
		stack->frames[stack->length-1]=dmx_frame_newwait(type,value,blend);
	}
	else
	{
		for(signed int x = stack->length-1; x > idx;x--)
		{
			stack->frames[x]=stack->frames[x-1];
		}
		stack->frames[idx]=dmx_frame_newwait(type,value,blend);
	}
}

dmx_frame* dmx_frame_newimg(struct dmx_img* img)
{
	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type=DMX_FRAME_IMAGE;
	frame->image.image=img;

	return frame;
}

void dmx_stack_add_imgframe(struct dmx_stack* stack,struct dmx_img* img)
{
	dmx_stack_add_imgframe_atidx(stack,img,-1);
}
void dmx_stack_add_imgframe_atidx(struct dmx_stack* stack,struct dmx_img* img,signed int idx)
{
	if(stack->length == stack->alloc)
	{
		dmx_stack_realloc_frames(stack);
	}

	stack->length++;
	if(idx == -1)
	{
		stack->frames[stack->length - 1]=dmx_frame_newimg(img);
	}
	else
	{
		for(signed int x = stack->length-1; x > idx;x--)
		{
			stack->frames[x]=stack->frames[x-1];
		}
		stack->frames[idx]=dmx_frame_newimg(img);
	}
}

void dmx_stack_del_imgframe(struct dmx_stack* stack,unsigned int idx)
{
	if(idx >= stack->length)
		return;

	dmx_frame* frame = stack->frames[idx];

	for (unsigned int c = idx ; c < stack->length - 1 ; c++ )
	{
		stack->frames[c] = stack->frames[c+1];
	}
	stack->frames[stack->length]=NULL;

	if(frame->type == DMX_FRAME_IMAGE)
	{
		dmx_img_free(frame->image.image);
	}
	free(frame);

	stack->length--;
}

unsigned int dmx_stack_frame_count(struct dmx_stack* stack)
{
	return stack->length;
}

dmx_frame* dmx_stack_frame_getbyidx(struct dmx_stack* stack,unsigned int idx)
{
	if(idx < stack->length)
	{
		return stack->frames[idx];
	}
	return NULL;
}

void dmx_stack_process(struct dmx_stack* stack)
{
	if(stack == NULL)
		return;

	if(stack->length == 0)
		return;

	if(stack->active == 0)
		return;

	if(stack->active > stack->length)
	{
		stack->active = 1;
	}

	unsigned long long curr = getmilis();

	unsigned int curr_active = stack->active;

	unsigned int mode = 0;

	unsigned int onlywait=1;
	for(unsigned int x=1; x <= stack->length ;x++)
	{
		if( stack->frames[x-1]->type != DMX_FRAME_WAIT )
		{
			onlywait=0;
			break;
		}
	}
	if(onlywait)
		return;

	float pct = 0.0f;

	for(unsigned int x=curr_active; x <= stack->length ;x++)
	{
		if( stack->frames[x-1]->type == DMX_FRAME_WAIT )
		{
			if(stack->frames[x-1]->wait.blend==1)
			{
				pct = ((curr - stack->lastframetime) == 0)?0.0f:((curr - stack->lastframetime)/(float)stack->frames[x-1]->wait.milis);
				if(pct > 1.0f) pct=1.0f;
				mode = 2;
			}
			else
			{
				mode = 1;
			}
			
			break;
		}
		
	}

	//printf("\nmode:%i pct:%f act:%i\n",mode,pct,curr_active);

	//scroll back
	if(stack->frames[curr_active-1]->type == DMX_FRAME_WAIT)
	{
		do
		{
			curr_active--;
			if(curr_active==0)
			{
				curr_active=stack->length;
			}
		}
		while(stack->frames[curr_active-1]->type != DMX_FRAME_WAIT);
		
		curr_active++;
		//printf("inc5\n");
		
		if(curr_active > stack->length)
		{
			//printf("-\n");
			curr_active=1;
		}
	}

	//printf("bs %i\n",curr_active);
	//replay
	if(
		(stack->frames[curr_active-1]->type == DMX_FRAME_IMAGE)||
		(stack->frames[curr_active-1]->type == DMX_FRAME_COMMAND)
	) 
	{
		do
		{
			if(stack->frames[curr_active-1]->type == DMX_FRAME_IMAGE)
			{
				dmx_frame* active_frame = stack->frames[(curr_active)-1];
				if(mode==2)
				{
					//printf("x %f\n",pct);
					dmx_img_render_pct(active_frame->image.image,1.0f-pct);
				}
				else
				{
					dmx_img_render(active_frame->image.image);
				}
			}
			else if(stack->frames[curr_active-1]->type == DMX_FRAME_COMMAND)
			{
				//execute command
				//
				//during blend execute to 50%
			}
			
			if(stack->frames[curr_active-1]->type != DMX_FRAME_WAIT)
			{
				curr_active++;
				//printf("inc4\n");

				if(curr_active > stack->length)
				{
					//printf("-\n");
					curr_active=1;

					if(mode == 0)
						return;
				}
			}

		}while( stack->frames[curr_active-1]->type != DMX_FRAME_WAIT );
	};

	if(stack->lastframetime == 0)
	{
		stack->lastframetime = curr;
	}

	//curr_active is implicit WAIT

	stack->active = curr_active;
	//printf("set a1 %i\n",curr_active);
	if( (curr - stack->lastframetime) > stack->frames[curr_active-1]->wait.milis )
	{
		curr_active++;

		//printf("inc3\n");
		if(curr_active > stack->length)
		{
			//printf("-\n");
			curr_active=1;
		}
		stack->lastframetime = curr;
		stack->active = curr_active;
		//printf("set a2 %i\n",curr_active);
	}
	else
	{
		curr_active++;

		//printf("inc1\n");
		if(curr_active > stack->length)
		{
			//printf("-\n");
			curr_active=1;
		}
	}

	//blend

	if(mode==2)
	{

		do
		{
			if(stack->frames[curr_active-1]->type == DMX_FRAME_IMAGE)
			{
				dmx_frame* active_frame = stack->frames[(curr_active)-1];
				dmx_img_render_pct_add(active_frame->image.image,pct,1);
			}
			else if(stack->frames[curr_active-1]->type == DMX_FRAME_COMMAND)
			{
				//execute command
				//
				//during blend execute starting 50%
			}

			if(stack->frames[curr_active-1]->type != DMX_FRAME_WAIT)
			{
				curr_active++;
				//printf("inc2\n");

				if(curr_active > stack->length)
				{
					//iprintf("-\n");
					curr_active=1;
				}
			}

		}while( stack->frames[curr_active-1]->type != DMX_FRAME_WAIT );
	}

}

