#include <cstring>//strlen
#include <cstdio>//printf,FILE*
#include <cstdlib>//atoi
#include <cassert>//assert
#include <cerrno>//errno
#include "event.hpp"

const char* event_evt::name="event";
const char* start_evt::name="start";
const char* finish_evt::name="finish";
const char* teleport_evt::name="teleport";
//const char* call_evt::name="call";
const char* gameover_evt::name="gameover";

int read_evt(const char* filename, vvevt_t* events) {
	//declare variables
	int res=0;
	char* fp_res=0;
	FILE* fp=fopen(filename, "rb");
	if(fp==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	unsigned int row=0, col=0;
	unsigned int to_row=0, to_col=0;
	char buffer[257];
	char* buffer_p=buffer;
	event_evt* event;
	
	//read start
	fp_res=fgets(buffer, 256, fp);
	if(fp_res==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	row=0;
	col=0;
	res=sscanf(buffer_p, "start %u %u", &row, &col);
	assert(res==2);
	event=new start_evt();
	((start_evt*)event)->row=row;
	((start_evt*)event)->col=col;
	events->at(row).at(col)=event;
	
	//read finish
	fp_res=fgets(buffer, 256, fp);
	if(fp_res==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	row=0;
	col=0;
	res=sscanf(buffer_p, "finish %u %u", &row, &col);
	assert(res==2);
	event=new finish_evt();
	((finish_evt*)event)->row=row;
	((finish_evt*)event)->col=col;
	events->at(row).at(col)=event;
	event=NULL;
	
	do {
		//read event
		fp_res=fgets(buffer, 256, fp);
		buffer_p=buffer;
		row=0;
		col=0;
		to_row=0;
		to_col=0;
		if(fp_res==NULL) {
			fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
			return 1;
		}
		if(strncmp(buffer, "end", 3)==0) break;
		if(strstr(buffer, "teleport")!=NULL) {
			res=sscanf(buffer_p, "%u %u teleport %u %u", &row, &col, &to_row, &to_col);
			assert(res==4);
			event=new teleport_evt();
			((teleport_evt*)event)->row=row;
			((teleport_evt*)event)->col=col;
			((teleport_evt*)event)->to_row=to_row;
			((teleport_evt*)event)->to_col=to_col;
			events->at(row).at(col)=event;
		}
		if(strstr(buffer, "gameover")!=NULL) {
			res=sscanf(buffer_p, "%u %u gameover", &row, &col);
			assert(res==2);
			event=new gameover_evt();
			((gameover_evt*)event)->row=row;
			((gameover_evt*)event)->col=col;
			events->at(row).at(col)=event;
		}
	}
	while(!feof(fp));
	fclose(fp);
	return 0;
}
//
