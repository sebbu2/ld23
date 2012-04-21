#ifndef EVENT_HPP
#define EVENT_HPP

#include "global.hpp"

struct event_evt {
	static const char* name;
};

struct start_evt: event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
};

struct finish_evt: event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
};

struct teleport_evt: event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
	unsigned int to_row;
	unsigned int to_col;
};

/*struct call_evt: event_evt {//later, if time allows it
	static const char* name;
	char* arg;
};//*/

struct gameover_evt: event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
};

int read_evt(const char* filename, vvevt_t* events);

#endif
//
