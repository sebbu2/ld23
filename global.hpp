#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <vector>

using namespace std;

extern const char *my_errors[];

extern const unsigned int status_height;
extern unsigned int width;
extern unsigned int height;

struct event_evt;

typedef vector<vector<int> > vvi_t;
typedef vector<vector<unsigned int> > vvui_t;
typedef vector<vector<event_evt*> > vvevt_t;

#endif
//
