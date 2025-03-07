#ifndef _FB_H
#define _FB_H

#include "core.h"

#define FB_GRID_WIDTH 	60
#define FB_GRID_HEIGHT 	60 

#define FB_DESIRED_DEPTH 		32
#define FB_DESIRED_WIDTH		1920
#define FB_DESIRED_HEIGHT 		1080
#define FB_DESIRED_ALIGNMENT 	7680

typedef struct _fb_info
{
	unsigned int 	width;
	unsigned int 	height;
	unsigned int    depth;
	unsigned int    pitch;
	unsigned int    alignment;
	void*  			buffer;
	unsigned int 	size;  
	bool 			is_rgb;	
}FB_INFO,*PFB_INFO;


bool fb_init();

void fb_draw_splash_image();

#endif