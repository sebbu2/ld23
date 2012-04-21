#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "global.hpp"
#include "position.hpp"

class SDL_Rect;

unsigned int get_collision(const vvui_t& level, unsigned int row, unsigned int col);

/*
#	L	R	LR
U	UL	U	ULR
D	DL	DR	DLR
UD	UDL	UDR	UDLR
*/

bool can_go_left(const vvui_t& level, unsigned int row, unsigned int col);
bool can_go_right(const vvui_t& level, unsigned int row, unsigned int col);
bool can_go_up(const vvui_t& level, unsigned int row, unsigned int col);
bool can_go_down(const vvui_t& level, unsigned int row, unsigned int col);

bool can_go_left(const vvui_t& level, SDL_Rect pos);
bool can_go_right(const vvui_t& level, SDL_Rect pos);
bool can_go_up(const vvui_t& level, SDL_Rect pos);
bool can_go_down(const vvui_t& level, SDL_Rect pos);

#endif
//
