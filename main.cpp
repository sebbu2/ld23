#include <cstring>
#include <cstdio>
// #include <cstdlib>
// #include <cstddef>
// #include <string>
#include <cassert>
#include <cerrno>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "main.hpp"
#include "level.hpp"
#include "position.hpp"
#include "collision.hpp"

int main(int argc, char* argv[]) {
	printf("argc==%d\n", argc);
	if(argc>=2) {
		if( strcmp(argv[1],"-v")==0 || strcmp(argv[1],"--version")==0 ) {
			printf("%s version 0.0.1\n", argv[0]);
			printf("sebbu's entry for LD23.\n");
		}
		return 0;
	}
	my_errors[(int)EAGAIN]="EAGAIN";
	my_errors[(int)EBADF]="EBADF";
	my_errors[(int)EINTR]="EINTR";
	my_errors[(int)EIO]="EIO";
	//my_errors[(int)EOVERFLOW]="EOVERFLOW";
	my_errors[(int)ENOMEM]="ENOMEM";
	my_errors[(int)ENXIO]="ENXIO";
	int error_code=0;
	
	//SDL init
	++error_code;//1
	if( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 ) {
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
	screen = SDL_SetVideoMode((int)width, (int)height, 32, SDL_SWSURFACE);
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
	Uint32 st_blue2=SDL_MapRGBA(screen->format,160,192,255,SDL_ALPHA_TRANSPARENT);
	printf("st_blue2 A : %u %u %u %u\n", st_blue2>>24, (st_blue2>>16)&0xFF, (st_blue2>>8)&0xFF, st_blue2&0xFF);
	Uint32 transparent=SDL_MapRGBA(screen->format, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
	printf("transparent A : %u %u %u %u\n", transparent>>24, (transparent>>16)&0xFF, (transparent>>8)&0xFF, transparent&0xFF);
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,transparent);
	
	//images
	SDL_Surface *fond=IMG_Load("world_001.png");
	SDL_Rect fond_pos={0,32,384,384};
	++error_code;//4
	if(fond==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	SDL_Surface *player=IMG_Load("player2.png");
	SDL_Rect player_pos={0,256,0,0};
	player_pos.x=(Sint16)(player_pos.x+fond_pos.x);
	player_pos.y=(Sint16)(player_pos.y+fond_pos.y);
	if(player==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	SDL_SetColorKey( player, SDL_SRCCOLORKEY, SDL_MapRGB(player->format, 255, 255, 255) );
	
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
	
	//load level
	vvui_t level;
	int res=read_lvl("world_001.lvl", &level);
	assert(res==1);
	printf("cell(row,col)=value\n");
	for(unsigned int i=0;i<level.size();++i) {//height
		for(unsigned int j=0;j<level.at(0).size();++j) {//width
			printf("%02d, ", level.at(i).at(j));
		}
		printf("\n");
	}//*/
	
	
	//SDL_EnableKeyRepeat(10, 10);
	SDL_EnableKeyRepeat(40, 20);
	
	//dialog
	/*Dialogue dlg("");
	ostringstream s;
	s << "Hello World!";
	dlg.set(s.str());//*/
	
	//game loop
	
	bool quitProgram=false;
#if 1
	bool playing=false;//music
	SDL_Event event;
#endif
#if 0
	Uint8 *state=NULL;
	int key_count=0;
	state = SDL_GetKeyState(&key_count);
	printf("[INFO] key count = %d\n", key_count);
	assert(key_count==SDLK_LAST);
#endif
	// duration of main loop
	Uint32 duration = 40;
	++error_code;//9
	/*
	0	L
	1	R
	2	U
	3	D
	*/
	/*
	-1	left
	0	normal (unset)
	+1	right
	*/
	char x_diff=0;
	char x_diff_previous=0;
	/*
	-1	up
	0	normal (unset)
	+1	down
	*/
	char y_diff=0;
	char y_diff_previous=0;
	x_diff_previous+=0;
	y_diff_previous+=0;
	while (!quitProgram)
	{
		Uint32 ticks = SDL_GetTicks();
		
#if 1
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
						(event.motion.x>4 && event.motion.x<108) &&
						(event.motion.y>4 && event.motion.y<48) &&
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
						case SDLK_LEFT:
							if(x_diff_previous>0) x_diff_previous=x_diff;
							x_diff=-1;
							y_diff= 0;y_diff_previous=0;
							break;
						case SDLK_RIGHT:
							if(x_diff_previous<0) x_diff_previous=x_diff;
							x_diff= 1;
							y_diff= 0;y_diff_previous=0;
							break;
						case SDLK_UP:
							if(y_diff_previous>0) y_diff_previous=y_diff;
							y_diff=-1;
							x_diff= 0;x_diff_previous=0;
							break;
						case SDLK_DOWN:
							if(y_diff_previous<0) y_diff_previous=y_diff;
							y_diff= 1;
							x_diff= 0;x_diff_previous=0;
							break;
						default:
							break;
					}//end switch key
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_LEFT:
							if(x_diff!=0) {
								assert(x_diff_previous>=0);
								x_diff=x_diff_previous;
							}
							x_diff_previous=0;
							break;
						case SDLK_RIGHT:
							if(x_diff!=0) {
								assert(x_diff_previous<=0);
								x_diff=x_diff_previous;
							}
							x_diff_previous=0;
							break;
						case SDLK_UP:
							if(y_diff!=0) {
								assert(y_diff_previous>=0);
								y_diff=y_diff_previous;
							}
							y_diff_previous=0;
							break;
						case SDLK_DOWN:
							if(y_diff!=0) {
								assert(y_diff_previous<=0);
								y_diff=y_diff_previous;
							}
							y_diff_previous=0;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}//end events
#endif

#if 0
		//events (bis)
		SDL_PumpEvents();
		if( state[SDLK_ESCAPE]!=0 ) {
			quitProgram=true;
		}
		if( (state[SDLK_LEFT]==0) ^ (state[SDLK_RIGHT]==0) ) {
			if( state[SDLK_LEFT]==1 ) {
				x_diff=-1;
			}
			if( state[SDLK_RIGHT]==1 ) {
				x_diff= 1;
			}
		}
		else {
			x_diff= 0;
		}
		if( (state[SDLK_UP]==0) ^ (state[SDLK_DOWN]==0) ) {
			if( state[SDLK_UP]==1 ) {
				y_diff=-1;
			}
			if( state[SDLK_DOWN]==1 ) {
				y_diff= 1;
			}
		}
		else {
			y_diff= 0;
		}
#endif
		
		//parse event
		if(x_diff!=0) {
			if(x_diff<0 && can_go_left(level, player_pos)) {
				player_pos.x=(Sint16)(player_pos.x+x_diff*8);
			}
			else if(x_diff>0 && can_go_right(level, player_pos)) {
				player_pos.x=(Sint16)(player_pos.x+x_diff*8);
			}
		}
		if(y_diff!=0) {
			if(y_diff<0 && can_go_up(level, player_pos)) {
				player_pos.y=(Sint16)(player_pos.y+y_diff*8);
			}
			else if(y_diff>0 && can_go_down(level, player_pos)) {
				player_pos.y=(Sint16)(player_pos.y+y_diff*8);
			}
		}
		
		//clear screen
		SDL_FillRect(screen, NULL, white); // white
		//SDL_FillRect(screen, NULL, black); // black
		//SDL_FillRect(screen, NULL, transparent); // transparent
		//SDL_FillRect(screen, NULL, st_blue); // st_blue
		
		//draw labyrinth
		SDL_BlitSurface(fond,NULL,screen,&fond_pos);
		
		//draw rounded box
		roundedBoxColor(screen, 4, 4, 108, 28, 4, st_blue);
		roundedBoxColor(screen, 6, 6, 106, 26, 4, st_blue2);
		
		//draw text
		SDL_Color _green={0,255,0,0};
		SDL_Surface *text_surface = TTF_RenderText_Blended(font, "HP: 100/100", _green);
		if(text_surface!=NULL) {
			SDL_Rect pos;
			pos.x=12;
			pos.y=12;
			SDL_BlitSurface(text_surface,NULL,screen,&pos);
			SDL_FreeSurface(text_surface);
		}
		
		//draw image
		{
			/*SDL_Rect pos_spr;
			pos_spr.x=0;
			pos_spr.y=64;
			pos_spr.w=32;
			pos_spr.h=32;*/
			SDL_BlitSurface(player,NULL,screen,&player_pos);
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
