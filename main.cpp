#include <cstring>
#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "main.hpp"

int main(int argc, char* argv[]) {
	printf("argc==%d\n", argc);
	if(argc>=2) {
		if( strcmp(argv[1],"-v")==0 || strcmp(argv[1],"--version")==0 ) {
			printf("%s version 0.0.1\n", argv[0]);
			printf("sebbu's entry for LD23.\n");
		}
		return 0;
	}
	int error_code=0;
	
	//SDL init
	++error_code;//1
	if( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(error_code);
	}
	atexit(SDL_Quit);
	
	++error_code;//2
	if(TTF_Init()==-1) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		exit(error_code);
	}
	
	//SDL video
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	++error_code;//3
	if ( screen == NULL ) {
		fprintf(stderr, "Unable to set 800x600 video: %s\n", SDL_GetError());
		exit(error_code);
	}
	
	//colors
	Uint32 black=SDL_MapRGB(screen->format,0,0,0);
	printf("black A : %u %u %u %u\n", black>>24, (black>>16)&0xFF, (black>>8)&0xFF, black&0xFF);
	Uint32 white=SDL_MapRGB(screen->format,255,255,255);
	printf("white A : %u %u %u %u\n", white>>24, (white>>16)&0xFF, (white>>8)&0xFF, white&0xFF);
	Uint32 st_blue=SDL_MapRGBA(screen->format,96,128,255,SDL_ALPHA_TRANSPARENT);
	printf("st_blue A : %u %u %u %u\n", st_blue>>24, (st_blue>>16)&0xFF, (st_blue>>8)&0xFF, st_blue&0xFF);
	Uint32 transparent=SDL_MapRGBA(screen->format, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
	printf("transparent A : %u %u %u %u\n", transparent>>24, (transparent>>16)&0xFF, (transparent>>8)&0xFF, transparent&0xFF);
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,transparent);
	
	//image
	SDL_Surface *fond=IMG_Load("152-Animal02.png");
	++error_code;//4
	if(fond==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	
	//font
	TTF_Font *font;
	font=TTF_OpenFont("courbd.ttf", 12);
	++error_code;//5
	if(!font) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		/* handle error */
		exit(error_code);
	}
	
	
	
	// load support for the music formats
	int flags=MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_FLAC;
	int initted=Mix_Init(flags);
	++error_code;//6
	if( (initted&flags) != flags) {
		fprintf(stderr, "Mix_Init: Failed to init required ogg and mod support!\n");
		fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
		// handle error
		exit(error_code);
	}
	
	++error_code;//7
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		// handle error
		exit(error_code);
	}
	
	Mix_Chunk *sample;
	sample=Mix_LoadWAV("test002.wav");
	++error_code;//8
	if(!sample) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		// handle error
		exit(error_code);
	}
	
	// allocate 16 mixing channels
	Mix_AllocateChannels(16);
	
	//dialog
	/*Dialogue dlg("");
	ostringstream s;
	s << "Hello World!";
	dlg.set(s.str());//*/
	
	//game loop
	
	SDL_Event event;
	
	bool quitProgram=false;
	bool playing=false;
	// duration of main loop
	Uint32 duration = 40;
	++error_code;//9
	while (!quitProgram)
	{
		Uint32 ticks = SDL_GetTicks();
		
		//start events
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEMOTION:
					/*printf("Mouse moved by %d,%d to (%d,%d)\n", 
						event.motion.xrel, event.motion.yrel,
						event.motion.x, event.motion.y);//*/
					break;
				case SDL_MOUSEBUTTONDOWN:
					/*printf("Mouse button %d pressed at (%d,%d)\n",
						event.button.button, event.button.x, event.button.y);//*/
					if(
						(event.motion.x>10 && event.motion.x<210) &&
						(event.motion.y>10 && event.motion.y<210) &&
						!playing ) {
							if(Mix_PlayChannel(-1, sample, 0)==-1) {
								fprintf(stderr, "Mix_PlayChannel: %s\n",Mix_GetError());
								// may be critical error, or maybe just no channels were free.
								// you could allocated another channel in that case...
								// handle error
								exit(error_code);
							}
						}
					break;
				case SDL_QUIT:
					quitProgram=true;
					//exit(0);
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE: // Escape pressed, exiting program
							quitProgram=true;
							break;
						default:
							break;
					}//end switch key
					break;
				default:
					break;
			}
		}//end events
		
		//clear screen
		SDL_FillRect(screen, NULL, white); // white
		//SDL_FillRect(screen, NULL, black); // black
		//SDL_FillRect(screen, NULL, transparent); // transparent
		//SDL_FillRect(screen, NULL, st_blue); // st_blue
		
		//draw rounded box
		roundedBoxColor(screen, 10, 10, 210, 210, 24, st_blue);
		
		//draw text
		SDL_Color _green={0,255,0,0};
		SDL_Surface *text_surface = TTF_RenderText_Blended(font, "Hello World!", _green);
		if(text_surface!=NULL) {
			SDL_Rect pos;
			pos.x=25;
			pos.y=25;
			SDL_BlitSurface(text_surface,NULL,screen,&pos);
			SDL_FreeSurface(text_surface);
		}
		
		//draw image
		{
			SDL_Rect pos;
			pos.x=225;
			pos.y=225;
			SDL_BlitSurface(fond,NULL,screen,&pos);
		}
		
		/* swap buffers */
		SDL_Flip(screen);
		
		/* sleep */
		Uint32 now = SDL_GetTicks();
		if (now < ticks + duration)
			SDL_Delay(ticks + duration - now);
	}
	
	//cleaning
	SDL_FreeSurface(screen);

	//SDLNet_Quit();
	Mix_FreeChunk(sample);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}
//
