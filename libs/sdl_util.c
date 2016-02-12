
#include "SDL.h"
#include "sdl_util.h"


static unsigned int* pixelbuffer;
static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;
static int initialized = 0;
static int fpsMill;
static int row;

unsigned int* sdl_init(int h, int v,const char* title, int fps)
{
	pixelbuffer = malloc(h*v*sizeof(uint32_t));
	memset(pixelbuffer, 0,h*v*sizeof(uint32_t));

	SDL_EnableScreenSaver();
	window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, h,v, SDL_WINDOW_SHOWN );
	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STATIC,h,v);
	fpsMill = 1000/fps;
	initialized=1;
	row=h;

	return pixelbuffer;
};


void sdl_deinit(void)
{
	initialized=0;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(pixelbuffer);
	SDL_Quit();
}


static int keymap;
static int releasemap;
static int ackmap;

int getkey(int key)
{
	if((keymap >> key) & 1)
	{
		ackmap |= (1 << key);
		return 1;
	}
	return 0;
}

void release_upped_keys(void)
{
	for(int i = 0; i < 8;i++)
	{
		if(releasemap & (1<<i))
		{
			keymap &= ~(1 << i);
		}
	}
	releasemap = 0;
}


static void keyup(int key)
{
	if(ackmap & (1<<key))
	{
		keymap &= ~(1 << key);
	}
	else
	{
		releasemap |= (1 << key);
	}
}

static void keydown(int key)
{
	keymap = 1 << key;
	ackmap &= ~(1 << key);
}

int init_tick;

int sdl_handle_events(const void* pixels)
{

	SDL_UpdateTexture(texture, NULL, pixels, row * sizeof(Uint32));//update only the updated rects
	//	SDL_RenderClear(renderer);//neccessary?
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// limit to fps
	int current_tick = SDL_GetTicks();
	if( (current_tick - init_tick) < fpsMill)
	{
		SDL_Delay(fpsMill - (current_tick - init_tick));
	}
	init_tick = SDL_GetTicks();

	SDL_Event ev;
	while(SDL_PollEvent(&ev)) 
	{
		switch(ev.type) {
			case SDL_QUIT:
				return 0;
				break;
			case SDL_KEYUP:
				switch(ev.key.keysym.sym) 
				{
					case SDLK_UP:
						keyup(0);
						break;
					case SDLK_RIGHT:
						keyup(1);
						break;
					case SDLK_DOWN:
						keyup(2);
						break;
					case SDLK_LEFT:
						keyup(3);
						break;
					case SDLK_F1:
						keyup(4);
						break;
					case SDLK_F2:
						keyup(5);
						break;
					case SDLK_F3:
						keyup(6);
						break;
					case SDLK_F4:
						keyup(7);
						break;
					default: break;
				}
				break;
			case SDL_KEYDOWN:
				switch(ev.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						return 0;
						break;
					case SDLK_UP:
						keydown(0);
						break;
					case SDLK_RIGHT:
						keydown(1);
						break;
					case SDLK_DOWN:
						keydown(2);
						break;
					case SDLK_LEFT:
						keydown(3);
						break;
					case SDLK_F1:
						keydown(4);
						break;
					case SDLK_F2:
						keydown(5);
						break;
					case SDLK_F3:
						keydown(6);
						break;
					case SDLK_F4:
						keydown(7);
						/*
						   zoom=20;
						   sdl_windowsize(CAVE_WIDTH*zoom, CAVE_HEIGHT*zoom);


						   free(pixelarray);
						   pixelarray = malloc (CAVE_HEIGHT *zoom * CAVE_WIDTH * zoom * sizeof(uint32_t));
						   memset(pixelarray,0,CAVE_HEIGHT *zoom * CAVE_WIDTH * zoom * sizeof(uint32_t));*/
						break;
					default: break;
				}
			default: break;
		}
	}
	return 1;

}
