#include <SDL.h>
#include "collision.hpp"

unsigned int get_collision(const vvui_t& level, unsigned int row, unsigned int col) {
	return level.at(row).at(col);
}

/*
#	L	R	LR
U	UL	U	ULR
D	DL	DR	DLR
UD	UDL	UDR	UDLR
*/

bool can_go_left(const vvui_t& level, unsigned int row, unsigned int col) {
	if(row>level.size()) return false;
	if(col>level.at(0).size()) return false;
	unsigned int src=get_collision(level, row, col);
	if(col==0) return false;
	unsigned int dst=get_collision(level, row, col-1);
	return (((src%2)==0)&&((dst%4)<2));
}
bool can_go_right(const vvui_t& level, unsigned int row, unsigned int col) {
	if(row>level.size()) return false;
	if(col>level.at(0).size()) return false;
	unsigned int src=get_collision(level, row, col);
	if((col+1)>=level.at(0).size()) return false;
	unsigned int dst=get_collision(level, row, col+1);
	return (((src%4)<2)&&((dst%2)==0));
}
bool can_go_up(const vvui_t& level, unsigned int row, unsigned int col) {
	if(row>level.size()) return false;
	if(col>level.at(0).size()) return false;
	unsigned int src=get_collision(level, row, col);
	if(row==0) return false;
	unsigned int dst=get_collision(level, row-1, col);
	return ((((src/4)%2)==0)&&((dst/8)==0));
}
bool can_go_down(const vvui_t& level, unsigned int row, unsigned int col) {
	if(row>level.size()) return false;
	if(col>level.at(0).size()) return false;
	unsigned int src=get_collision(level, row, col);
	if((row+1)>=level.size()) return false;
	unsigned int dst=get_collision(level, row+1, col);
	return (((src/8)==0)&&(((dst/4)%2)==0));
}

/*
#	L	R	LR
U	UL	U	ULR
D	DL	DR	DLR
UD	UDL	UDR	UDLR
*/

bool can_go_left(const vvui_t& level, SDL_Rect pos) {
	unsigned int row=get_row(pos);
	bool row_m=(((unsigned int)pos.y-status_height-row*32)!=0);
	unsigned int col=get_col_c(pos);
	//bool col_m=(((unsigned int)pos.x-col*32)!=0);
	bool res=true;
	if(row_m) {
		res&=can_go_left(level, row, col);
		if(!res) printf("[INFO] l1 first\n");
		res&=can_go_left(level, row+1, col);
		if(!res) printf("[INFO] l1 second\n");
		res&=can_go_down(level, row, col-1);
		if(!res) printf("[INFO] l1 third\n");
		res&=can_go_up(level, row+1, col-1);
		if(!res) printf("[INFO] l1 fourth\n");
	}
	else {
		res&=can_go_left(level, row, col);
		if(!res) printf("[INFO] l2 first\n");
	}
	return res;
}

bool can_go_right(const vvui_t& level, SDL_Rect pos) {
	unsigned int row=get_row(pos);
	bool row_m=(((unsigned int)pos.y-status_height-row*32)!=0);
	unsigned int col=get_col(pos);
	//bool col_m=(((unsigned int)pos.x-col*32)!=0);
	bool res=true;
	if(row_m) {
		res&=can_go_right(level, row, col);
		if(!res) printf("[INFO] r1 first\n");
		res&=can_go_right(level, row+1, col);
		if(!res) printf("[INFO] r1 second\n");
		res&=can_go_down(level, row, col+1);
		if(!res) printf("[INFO] r1 third\n");
		res&=can_go_up(level, row+1, col+1);
		if(!res) printf("[INFO] r1 fourth\n");
	}
	else {
		res&=can_go_right(level, row, col);
		if(!res) printf("[INFO] r2 first\n");
	}
	return res;
}

bool can_go_up(const vvui_t& level, SDL_Rect pos) {
	unsigned int row=get_row_c(pos);
	//bool row_m=(((unsigned int)pos.y-status_height-row*32)!=0);
	unsigned int col=get_col(pos);
	bool col_m=(((unsigned int)pos.x-col*32)!=0);
	bool res=true;
	if(col_m) {
		res&=can_go_up(level, row, col);
		if(!res) printf("[INFO] u1 first\n");
		res&=can_go_up(level, row, col+1);
		if(!res) printf("[INFO] u1 second\n");
		res&=can_go_right(level, row-1, col);
		if(!res) printf("[INFO] u1 third\n");
		res&=can_go_left(level, row-1, col+1);
		if(!res) printf("[INFO] u1 fourth\n");
	}
	else {
		res&=can_go_up(level, row, col);
		if(!res) printf("[INFO] u2 first\n");
	}
	return res;
}

bool can_go_down(const vvui_t& level, SDL_Rect pos) {
	unsigned int row=get_row(pos);
	//bool row_m=(((unsigned int)pos.y-status_height-row*32)!=0);
	unsigned int col=get_col(pos);
	bool col_m=(((unsigned int)pos.x-col*32)!=0);
	bool res=true;
	if(col_m) {
		res&=can_go_down(level, row, col);
		if(!res) printf("[INFO] u1 first\n");
		res&=can_go_down(level, row, col+1);
		if(!res) printf("[INFO] u1 second\n");
		res&=can_go_right(level, row+1, col);
		if(!res) printf("[INFO] u1 third\n");
		res&=can_go_left(level, row+1, col+1);
		if(!res) printf("[INFO] u1 fourth\n");
	}
	else {
		res&=can_go_down(level, row, col);
		if(!res) printf("[INFO] u2 first\n");
	}
	return res;
}
//
