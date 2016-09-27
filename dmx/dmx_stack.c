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
	stack->active=0;
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

void dmx_stack_free(struct dmx_stack* stack)
{
	for (unsigned int c = 0 ; c < stack->length; c++ )
	{
		if(stack->frames[c]->type == DMX_FRAME_IMAGE)
		{
			dmx_img_free(stack->frames[c]->image.image);
		}
		free(stack->frames[c]);
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

				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->wait.milis);
				SDL_WriteBE32(file,dmx_stack_list[i]->frames[c]->wait.bpms);

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
	printf("file: %s\n",file_path);
	SDL_RWops *file = SDL_RWFromFile(file_path, "r");
	free(file_path);

	if(file == NULL)
		return;


	unsigned int stacks = SDL_ReadBE32(file);


	printf("stacks: %i\n",stacks);

	for(unsigned int x = 0; x < stacks;x++)
	{
		struct dmx_stack* stack = malloc(sizeof(struct dmx_stack));

		printf("stack:\n");

		unsigned int name_length = SDL_ReadU8(file);
		if(name_length > (DMX_NAME_LENGTH-1)) name_length=DMX_NAME_LENGTH-1;
		unsigned int xy = SDL_RWread(file, stack->name, 1, name_length+1);
		printf("namelen: %i\n",xy);
		stack->active=0;
		stack->length=0;

		stack->alloc=SDL_ReadBE32(file);// if we define a limit later, enforce it here
		stack->frames=malloc(sizeof(dmx_frame*)*stack->alloc);

		printf("alloc: %i\n",stack->alloc);

		for(unsigned int c=0;c<stack->alloc;c++)
		{
			unsigned int frame_type = SDL_ReadBE32(file);

			printf("ft: %i\n",frame_type);

			if(frame_type == DMX_FRAME_IMAGE)
			{

				struct dmx_img* image = dmx_img_new();

				unsigned int dev_count = SDL_ReadBE32(file);

				printf("dev: %i\n",dev_count);// if we define a limit later, enforce it here

				for(unsigned int x = 0;x<dev_count;x++)
				{
					unsigned int dev_name_length = SDL_ReadU8(file);
					printf("devnl: %i\n",dev_name_length);
					char input[DMX_NAME_LENGTH];
					if(dev_name_length > (DMX_NAME_LENGTH-1)) dev_name_length=DMX_NAME_LENGTH-1;
					SDL_RWread(file, input, 1, dev_name_length+1);
					dmx_img_device_add(image, input);
				}

				image->is_dim = SDL_ReadBE32(file);//todo: sanity check
				image->is_col = SDL_ReadBE32(file);//todo: sanity check

				unsigned int float_length = SDL_ReadU8(file);
				printf("float nl: %i\n",float_length);
				char floatinput[50];
				if(float_length > 49) float_length=49;
				SDL_RWread(file, floatinput, 1, float_length+1);

				image->dim = atof(floatinput);

				printf("dim: %f\n",image->dim);

				image->r = SDL_ReadBE32(file);
				image->g = SDL_ReadBE32(file);
				image->b = SDL_ReadBE32(file);

				unsigned int col_name_length = SDL_ReadU8(file);
				printf("color nl: %i\n",col_name_length);
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

				unsigned int milis = SDL_ReadBE32(file);
				unsigned int bpms = SDL_ReadBE32(file);

				if(milis>0)
				{
					dmx_stack_add_waitframe(stack,0,milis);
				}
				else
				{
					dmx_stack_add_waitframe(stack,1,bpms);
				}

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

void dmx_stack_add_waitframe(struct dmx_stack* stack,unsigned int type,unsigned int value)
{
	if(stack->length == stack->alloc)
	{
		dmx_stack_realloc_frames(stack);
	}

	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type=DMX_FRAME_WAIT;
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

	stack->frames[stack->length]=frame;
	stack->length++;
}

void dmx_stack_add_imgframe(struct dmx_stack* stack,struct dmx_img* img)
{
	if(stack->length == stack->alloc)
	{
		dmx_stack_realloc_frames(stack);
	}

	dmx_frame* frame = malloc(sizeof(dmx_frame));
	frame->type=DMX_FRAME_IMAGE;
	frame->image.image=img;

	stack->frames[stack->length]=frame;
	stack->length++;
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

	unsigned int done = 0;

	unsigned long long curr = getmilis();

	unsigned int curr_active = stack->active;

	do
	{
		dmx_frame* active_frame = stack->frames[(curr_active)-1];

		if(active_frame->type == DMX_FRAME_IMAGE)
		{
			dmx_img_render(active_frame->image.image);
			curr_active++;
		}
		else if(active_frame->type == DMX_FRAME_WAIT)
		{
			if( (curr - stack->lastframetime) > active_frame->wait.milis )
			{
				curr_active++;
				stack->active = curr_active;
				stack->lastframetime = curr;
			}
			done=1;
		}
		else if(active_frame->type == DMX_FRAME_COMMAND)
		{
		}

		if(curr_active > stack->length)
		{
			done=1;
			stack->active = 1;
		}

	}
	while(done==0);

}

