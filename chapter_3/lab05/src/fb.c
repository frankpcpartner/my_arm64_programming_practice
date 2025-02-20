#include "fb.h"
#include "mailbox.h"
#include "util.h"



FB_INFO gfb_info = 
{
	.width = FB_DESIRED_WIDTH,
	.height = FB_DESIRED_HEIGHT,
	.depth = FB_DESIRED_DEPTH,
	.pitch = 0,
	.alignment = FB_DESIRED_ALIGNMENT,
	.buffer = NULL,
	.is_rgb = true,
};


bool fb_init()
{

	if(mailbox_init_framebuffer(&gfb_info))
	{
		printf("fb info: w:%u,h:%u,depth:%u,pitch:%u,is_rgb:%u,buffer:%llu,size:%u\r\n",
			gfb_info.width,gfb_info.height,
			gfb_info.depth,gfb_info.pitch,
			gfb_info.is_rgb,gfb_info.buffer,
			gfb_info.size);

		if(gfb_info.width != FB_DESIRED_WIDTH || 
			gfb_info.height != FB_DESIRED_HEIGHT ||
			gfb_info.depth != FB_DESIRED_DEPTH ||
			gfb_info.pitch != FB_DESIRED_ALIGNMENT)
		{
			printf("Failed to alloc %ux%ux%u frame buffer.",FB_DESIRED_WIDTH,FB_DESIRED_HEIGHT,FB_DESIRED_DEPTH);
			return false;
		}
		return true;
	}
	else
	{

		printf("failed to call fb_init. \r\n");
	}

	return false;
}

void fb_draw_splash_image()
{
	if(gfb_info.buffer == NULL)
		return;

	//Black and white
	unsigned int required_color[] = {0xffffffff,0x00000000};
	unsigned int size = sizeof(required_color) / sizeof(unsigned int);
	int idx = 0;

	for(unsigned int h = 0; h < gfb_info.height; h += FB_GRID_HEIGHT)
	{
		int start_idx = idx;
		
		for(unsigned int w = 0; w < gfb_info.width; w += FB_GRID_WIDTH)
		{
			unsigned int* start_fb = (unsigned int*)(gfb_info.buffer) + h * gfb_info.width + w;
 			
			int color_idx = start_idx % size;	
 			
 			int grid_width = (w + FB_GRID_WIDTH) <=  gfb_info.width ? 
 				FB_GRID_WIDTH : gfb_info.width - w;

			int grid_height = (h + FB_GRID_HEIGHT) <= gfb_info.height ?
				FB_GRID_HEIGHT :  gfb_info.height - h;				


 			for(unsigned int g_w = 0; g_w < grid_width; g_w++)
 			{
 				for(unsigned int g_h = 0; g_h < grid_height; g_h++)
 				{
 					unsigned int* pixel = start_fb + g_h * gfb_info.width + g_w;			
 					(*pixel) = required_color[color_idx];	
 				}			
 			}

 			start_idx++;
		}

		idx++;	
	}	
}