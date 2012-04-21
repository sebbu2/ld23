#include <cstring>//strlen
#include <cstdio>//printf,FILE*
#include <cstdlib>//atoi
#include <cassert>//assert
#include <cerrno>//errno
#include "level.hpp"

int read_lvl(const char* filename, vvui_t* level) {
	//declare variables
	int res=0;
	char* fp_res=0;
	FILE* fp=fopen(filename, "rb");
	if(fp==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	char buffer[577];
	char* buffer_p=buffer;
	
	//read height
	fp_res=fgets(buffer, 576, fp);
	if(fp_res==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	unsigned int l_height=(unsigned)atoi(buffer);
	
	//read width
	fp_res=fgets(buffer, 576, fp);
	if(fp_res==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	unsigned int l_width=(unsigned)atoi(buffer);
	
	//allocate level
	level->resize(l_height);
	for(unsigned int i=0;i<l_height;++i) {
		level->at(i).resize(l_width);
	}
	
	//read level data
#if 0
	fp_res=fgets(buffer, 576, fp);
	if(fp_res==NULL) {
		fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
		return 1;
	}
	unsigned int buf_len=strlen(buffer);
	assert( (buf_len >= 3*l_height*l_width) && (buf_len <= 4*l_height*l_width) );
#endif
	
	//parse level data
	unsigned short nb=0;
	for(unsigned int i=0;i<l_height;++i) {
#if 1
		fp_res=fgets(buffer, 576, fp);
		buffer_p=buffer;
		if(fp_res==NULL) {
			fprintf(stderr, "[ERROR] %d - %s\n", errno, my_errors[errno]);
			return 1;
		}
		unsigned int buf_len=(unsigned int)strlen(buffer);
		assert( (buf_len >= 3*l_width) && (buf_len <= 4*l_width) );
#endif
		for(unsigned int j=0;j<l_width;++j) {
			nb=0;
			res=sscanf(buffer_p, "%hu", &nb);
			//assert(nb>0);//should not be 0??
			assert(nb<=16);
			assert(res==1);
			if(nb>0) --nb;//0 to 15, 4bit
			level->at(i).at(j)=(unsigned)nb;
			
			buffer_p+=3;
			if(nb>=9) ++buffer_p;
		}
	}
	
	//quit
	fclose(fp);
	return ((res==1)?0:1);//change value
}
//
