#include <SDL.h>
#include "position.hpp"

/*
#	L	R	LR
U	UL	U	ULR
D	DL	DR	DLR
UD	UDL	UDR	UDLR
*/

unsigned int get_row(const SDL_Rect pos) {
	return (unsigned)(((unsigned)pos.y-status_height)/32);
}
unsigned int get_row_f(const SDL_Rect pos) {
	return (unsigned)(((unsigned)pos.y-status_height)/32);
}
unsigned int get_row_c(const SDL_Rect pos) {
	return (unsigned)(((unsigned)pos.y+31-status_height)/32);
}

unsigned int get_col(const SDL_Rect pos) {
	return (unsigned)((pos.x)/32);
}
unsigned int get_col_f(const SDL_Rect pos) {
	return (unsigned)((pos.x)/32);
}
unsigned int get_col_c(const SDL_Rect pos) {
	return (unsigned)((pos.x+31)/32);
}
//
