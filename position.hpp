#ifndef POSITION_HPP
#define POSITION_HPP

#include "global.hpp"

class SDL_Rect;

unsigned int get_row(const SDL_Rect pos);
unsigned int get_row_f(const SDL_Rect pos);
unsigned int get_row_c(const SDL_Rect pos);

unsigned int get_col(const SDL_Rect pos);
unsigned int get_col_f(const SDL_Rect pos);
unsigned int get_col_c(const SDL_Rect pos);

#endif
//
