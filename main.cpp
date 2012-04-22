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
#ifndef NOAUDIO
#include <SDL_mixer.h>
#endif

#include "main.hpp"

SDL_Color get_color(Uint32 color) {
	SDL_Color res;
	res.r=(Uint8)((color>>16)&0xFF);
	res.g=(Uint8)((color>>8)&0xFF);
	res.b=(Uint8)(color&0xFF);
	res.unused=0;
	return res;
}

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
#ifndef NOAUDIO
	if( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 ) {
#else
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 ) {
#endif
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
	Uint32 black		=SDL_MapRGBA(screen->format,	0	,	0	,	0	,	SDL_ALPHA_TRANSPARENT);
	Uint32 white		=SDL_MapRGBA(screen->format,	255	,	255	,	255	,	SDL_ALPHA_TRANSPARENT);
	Uint32 st_blue		=SDL_MapRGBA(screen->format,	96	,	128	,	255	,	SDL_ALPHA_TRANSPARENT);
	Uint32 st_blue2		=SDL_MapRGBA(screen->format,	160	,	192	,	255	,	SDL_ALPHA_TRANSPARENT);
	Uint32 red			=SDL_MapRGBA(screen->format,	255	,	0	,	0	,	SDL_ALPHA_TRANSPARENT);
	Uint32 green		=SDL_MapRGBA(screen->format,	0	,	255	,	0	,	SDL_ALPHA_TRANSPARENT);
	Uint32 blue			=SDL_MapRGBA(screen->format,	0	,	0	,	255	,	SDL_ALPHA_TRANSPARENT);
	Uint32 yellow		=SDL_MapRGBA(screen->format,	255	,	255	,	0	,	SDL_ALPHA_TRANSPARENT);
	Uint32 transparent	=SDL_MapRGBA(screen->format,	255	,	255	,	255	,	255);
	printf("transparent A : %u %u %u %u\n", transparent>>24, (transparent>>16)&0xFF, (transparent>>8)&0xFF, transparent&0xFF);
	SDL_SetColorKey(screen, SDL_SRCCOLORKEY, transparent);
	assert( (black&0xFF000000)==0 && (white&0xFF000000)==0 && (st_blue&0xFF000000)==0 && (st_blue2&0xFF000000)==0 );
	assert( (red&0xFF000000)==0 && (green&0xFF000000)==0 && (blue&0xFF000000)==0 && (yellow&0xFF000000)==0 );
	SDL_Color _black=get_color(black);
	SDL_Color _white=get_color(white);
	SDL_Color _st_blue=get_color(st_blue);
	SDL_Color _st_blue2=get_color(st_blue2);
	SDL_Color _red=get_color(red);
	SDL_Color _green=get_color(green);
	SDL_Color _blue=get_color(blue);
	SDL_Color _yellow=get_color(yellow);
	assert( _black.unused==0 && _white.unused==0 && _st_blue.unused==0 && _st_blue2.unused==0 );
	assert( _red.unused==0 && _green.unused==0 && _blue.unused==0 && _yellow.unused==0 );
	
	//images
	//level
	SDL_Surface *fond=IMG_Load("world_001.png");
	SDL_Rect fond_pos={0,32,384,384};
	++error_code;//4
	if(fond==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	//player
	SDL_Surface *player=IMG_Load("player2.png");
	SDL_Rect player_pos={0,256,0,0};
	player_pos.x=(Sint16)(player_pos.x+fond_pos.x);
	player_pos.y=(Sint16)(player_pos.y+fond_pos.y);
	if(player==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	SDL_SetColorKey( player, SDL_SRCCOLORKEY, SDL_MapRGB(player->format, 255, 255, 255) );
	//shadow
	SDL_Surface *shadow=IMG_Load("mask.png");
	SDL_Rect shadow_pos={0,0,0,0};
	if(shadow==NULL) {
		fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
		exit(error_code);
	}
	SDL_SetColorKey( shadow, SDL_SRCCOLORKEY, SDL_MapRGB(shadow->format, 255, 255, 255) );
	
	//font
	TTF_Font *font;
	font=TTF_OpenFont("courbd.ttf", 12);
	++error_code;//5
	if(!font) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		/* handle error */
		exit(error_code);
	}
	
#ifndef NOAUDIO
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
#endif
	
	//load level
	vvui_t level;
	int res=read_lvl("world_001.lvl", &level);
	assert(res==0);
	printf("cell(row,col)=value\n");
	for(unsigned int i=0;i<level.size();++i) {//row
		for(unsigned int j=0;j<level.at(0).size();++j) {//col
			printf("%02d, ", level.at(i).at(j));
		}
		printf("\n");
	}//*/
	
	//load events
	vvevt_t events;
	events.resize(level.size());
	for(unsigned int i=0;i<events.size();++i) {//row
		events.at(i).resize(events.size());
	}
	res=read_evt("world_001.evt", &events);
	assert(res==0);
	event_evt* map_event;
	for(unsigned int i=0;i<events.size();++i) {//row
		for(unsigned int j=0;j<events.at(0).size();++j) {//col
			map_event=events.at(i).at(j);
			if(map_event==NULL) continue;
			if(strcmp(map_event->getName(),"start")==0) {
				printf("[INFO] start is at (%02d,%02d)\n", ((start_evt*)events.at(i).at(j))->row, ((start_evt*)events.at(i).at(j))->col);
			}
		}
	}
	
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
	bool in_move=false;
	unsigned int cheat=0;
	unsigned int map_height=level.size();
	unsigned int map_width=level.at(0).size();
	printf("[INFO] level is composed of %02d rows of %02d cols\n", map_height, map_width);
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
#ifndef NOAUDIO
							if(Mix_PlayChannel(-1, sample, 0)==-1) {
								fprintf(stderr, "Mix_PlayChannel: %s\n",Mix_GetError());
								// may be critical error, or maybe just no channels were free.
								// you could allocated another channel in that case...
								// handle error
								exit(error_code);
							}
#endif
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
						case SDLK_c:
							if((cheat&0x1F)==0x00) cheat=((cheat&0xC0)|(cheat&0x3F)|0x01);
							else cheat&=0xE0;
							break;
						case SDLK_h:
							if((cheat&0x1F)==0x01) cheat=((cheat&0xC0)|(cheat&0x3F)|0x02);
							else cheat&=0xE0;
							break;
						case SDLK_e:
							if((cheat&0x1F)==0x03) cheat=((cheat&0xC0)|(cheat&0x3F)|0x04);
							else cheat&=0xE0;
							break;
						case SDLK_q://qwerty ??
							if((cheat&0x1F)==0x07) cheat=((cheat&0xC0)|(cheat&0x3F)|0x08);
							else cheat&=0xE0;
							break;
						case SDLK_t:
							if((cheat&0x1F)==0x0F) cheat=((cheat&0xC0)|(cheat&0x3F)|0x10);
							else cheat&=0xE0;
							if((cheat&0x1F)==0x1F) cheat=((cheat&0xE0)^0x20);
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
				in_move=true;
			}
			else if(x_diff>0 && can_go_right(level, player_pos)) {
				player_pos.x=(Sint16)(player_pos.x+x_diff*8);
				in_move=true;
			}
		}
		if(y_diff!=0) {
			if(y_diff<0 && can_go_up(level, player_pos)) {
				player_pos.y=(Sint16)(player_pos.y+y_diff*8);
				in_move=true;
			}
			else if(y_diff>0 && can_go_down(level, player_pos)) {
				player_pos.y=(Sint16)(player_pos.y+y_diff*8);
				in_move=true;
			}
		}
		
		//launch map event
		if(((unsigned int)player_pos.y-status_height)%32==0 && player_pos.x%32==0) {
			if(in_move) {
				map_event=events.at(get_row(player_pos)).at(get_col(player_pos));
				if(map_event!=NULL) {
					//in_move=false;
					if(strcmp(map_event->getName(),"teleport")==0) {
						player_pos.x=(Sint16)(((teleport_evt*)map_event)->to_col*32);
						player_pos.y=(Sint16)(((teleport_evt*)map_event)->to_row*32+status_height);
					}
					else if(strcmp(map_event->getName(),"gameover")==0) {
						printf("Sorry, you lost, you were trapped and died.\n\nSee you again !\n");
						quitProgram=true;
					}
					else if(strcmp(map_event->getName(),"start")==0) {
						//do nothing
					}
					else if(strcmp(map_event->getName(),"finish")==0) {
						printf("Great, you won !\n\nSee you again !\n");
						quitProgram=true;
					}
					else {
						fprintf(stderr, "[DEBUG] unknown event '%s' on cell (%02d,%02d)\n", map_event->getName(), get_row(player_pos), get_col(player_pos));
					}
				}
			}
		}
		if(in_move) {
			x_diff=0;
			y_diff=0;
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
		SDL_Surface *text_surface = TTF_RenderText_Blended(font, "HP: 100/100", _green);
		if(text_surface!=NULL) {
			SDL_Rect pos;
			pos.x=12;
			pos.y=12;
			SDL_BlitSurface(text_surface,NULL,screen,&pos);
			SDL_FreeSurface(text_surface);
		}
		
		//draw events (if cheat)
		if((cheat&0x20)==0x20) {
			for(unsigned int i=0;i<events.size();++i) {//row
				for(unsigned int j=0;j<events.at(0).size();++j) {//col
					map_event=events.at(i).at(j);
					if(map_event==NULL) continue;
					short int x=(short)(j*32+2);
					short int y=(short)(i*32+status_height+2);
					short int w=(short)(32-4);
					short int h=(short)(32-4);
					if(strcmp(map_event->getName(),"start")==0) {
						roundedBoxColor(screen, x, y, (short)(x+w), (short)(y+h), 4, yellow<<8|0xFF);
					}
					else if(strcmp(map_event->getName(),"finish")==0) {
						roundedBoxColor(screen, x, y, (short)(x+w), (short)(y+h), 4, green<<8|0xFF);
					}
					else if(strcmp(map_event->getName(),"gameover")==0) {
						roundedBoxColor(screen, x, y, (short)(x+w), (short)(y+h), 4, red<<8|0xFF);
					}
					else if(strcmp(map_event->getName(),"teleport")==0) {
						roundedBoxColor(screen, x, y, (short)(x+w), (short)(y+h), 4, blue<<8|0xFF);
					}
				}
			}
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
		
		//draw shadow
		{
			shadow_pos.x=(short)(player_pos.x-32);
			shadow_pos.y=(short)(player_pos.y-32);
			if(shadow_pos.y<(short)status_height) {
				SDL_Rect pos_spr;
				pos_spr.x=0;
				pos_spr.y=(Sint16)(32-shadow_pos.y);
				pos_spr.w=96;
				pos_spr.h=(unsigned short)(96-shadow_pos.y);
				shadow_pos.y=32;
				SDL_BlitSurface(shadow,&pos_spr,screen,&shadow_pos);
			}
			else {
				SDL_BlitSurface(shadow,NULL,screen,&shadow_pos);
			}
			
			//if(in_move) {
			
				short point_start_x=0;
				short point_start_y=(short)status_height;
				short point_end_x=(short)(map_width*32);
				short point_end_y=(short)(status_height+map_height*32);
				short point_player_l_x=(short)player_pos.x;
				short point_player_u_y=(short)player_pos.y;
				short point_player_r_x=(short)(player_pos.x+32);
				short point_player_d_y=(short)(player_pos.y+32);
				
				if(in_move) {
					printf("[INFO] start     is at (%02d,%02d)\n", point_start_x, point_start_y);
					printf("[INFO] end       is at (%02d,%02d)\n", point_end_x, point_end_y);
					printf("[INFO] player UL is at (%02d,%02d)\n", point_player_l_x, point_player_u_y);
					printf("[INFO] player UR is at (%02d,%02d)\n", point_player_r_x, point_player_u_y);
					printf("[INFO] player DL is at (%02d,%02d)\n", point_player_l_x, point_player_d_y);
					printf("[INFO] player DR is at (%02d,%02d)\n", point_player_r_x, point_player_d_y);
				}
				
#define cond_width()	rect.w>0
#define cond_height()	rect.h>0

// #define cond_left()		(unsigned)player_pos.x>0
// #define cond_right()	map_width*32>(unsigned)player_pos.x
// #define cond_up()		(unsigned)player_pos.y>status_height+8
// #define cond_down()		map_height*32>(unsigned)player_pos.y
#define cond_left()		point_player_l_x>0
#define cond_right()	point_player_r_x<point_end_x
#define cond_up()		point_player_u_y>point_start_y
#define cond_down()		point_player_d_y<point_end_y
				
				SDL_Rect rect;
				
				//U
				rect.x=point_player_l_x;
				rect.y=point_start_y;
				rect.w=(Uint16)32;
				rect.h=(Uint16)(point_player_u_y-rect.y-8);
				
				if( cond_width() && cond_height() && cond_up() ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//D
				rect.x=point_player_l_x;
				rect.y=(Sint16)(point_player_d_y+8);
				rect.w=(Uint16)32;
				rect.h=(Uint16)(point_end_y-rect.y);
				
				if( cond_width() && cond_height() && cond_down() ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//L
				rect.x=point_start_x;
				rect.y=point_player_u_y;
				rect.w=(Uint16)(point_player_l_x-8);
				rect.h=(Uint16)32;
				
				if( cond_width() && cond_height() && cond_left() ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//R
				rect.x=(Sint16)(point_player_r_x+8);
				rect.y=point_player_u_y;
				rect.w=(Uint16)(point_end_x-rect.x);
				rect.h=(Uint16)32;
				
				if( cond_width() && cond_height() && cond_right() ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//UL
				rect.x=point_start_x;
				rect.y=point_start_y;
				rect.w=(Uint16)(point_player_l_x-rect.x);
				rect.h=(Uint16)(point_player_u_y-rect.y);
				
				if( cond_width() && cond_height() && (cond_up() || cond_left()) ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//UR
				rect.x=point_player_r_x;
				rect.y=point_start_y;
				rect.w=(Uint16)(point_end_x-rect.x);
				rect.h=(Uint16)(point_player_u_y-rect.y);
				
				if( cond_width() && cond_height() && (cond_up() || cond_right()) ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//DL
				rect.x=point_start_x;
				rect.y=point_player_d_y;
				rect.w=(Uint16)(point_player_l_x-rect.x);
				rect.h=(Uint16)(point_end_y-rect.y);
				
				if( cond_width() && cond_height() && (cond_down() || cond_left()) ) {
					SDL_FillRect(screen, &rect, black);
				}
				
				//DR
				rect.x=point_player_r_x;
				rect.y=point_player_d_y;
				rect.w=(Uint16)(point_end_x-rect.x);
				rect.h=(Uint16)(point_end_y-rect.y);
				
				if( cond_width() && cond_height() && (cond_down() || cond_right()) ) {
					SDL_FillRect(screen, &rect, black);
				}
				
			//}
		}
		
		if(x_diff==0 && y_diff==0) {
			in_move=false;
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
	SDL_FreeSurface(player);
	SDL_FreeSurface(shadow);
	SDL_FreeSurface(fond);

	//SDLNet_Quit();
#ifndef NOAUDIO
	Mix_FreeChunk(sample);
	Mix_CloseAudio();
	Mix_Quit();
#endif
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}
//
