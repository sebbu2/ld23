#ifndef EVENT_HPP
#define EVENT_HPP

#include "global.hpp"

struct event_evt {
	static const char* name;
	virtual const char* getName() const;
};

struct start_evt: public event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
	virtual const char* getName() const;
};

struct finish_evt: public event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
	virtual const char* getName() const;
};

struct teleport_evt: public event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
	unsigned int to_row;
	unsigned int to_col;
	virtual const char* getName() const;
};

/*struct call_evt: public event_evt {//later, if time allows it
	static const char* name;
	char* arg;
	virtual const char* getName() const;
};//*/

struct gameover_evt: public event_evt {
	static const char* name;
	unsigned int row;
	unsigned int col;
	virtual const char* getName() const;
};

int read_evt(const char* filename, vvevt_t* events);

#endif
//
