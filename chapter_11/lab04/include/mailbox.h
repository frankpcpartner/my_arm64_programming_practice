#ifndef _MAILBOX_H
#define _MAILBOX_H

#include "core.h"
#include "fb.h"

#define MAILBOX_BASE    (MMIO_BASE + 0x0000b880)

#define MAILBOX_READ    MAILBOX_BASE
#define MAILBOX_STATUS  MAILBOX_BASE + 0x18
#define MAILBOX_WRITE   MAILBOX_BASE + 0x20

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define MAILBOX_GET_BOARD_REVISION  	0x00010002
#define MAILBOX_GET_ARM_MEMORY  		0x00010005
#define MAILBOX_GET_VC_MEMORY  			0x00010006

#define MAILBOX_GET_CLOCK_STATE		0x00030001
#define MAILBOX_SET_CLOCK_STATE		0x00038001
#define MAILBOX_GET_CLOCK_RATE		0x00030002
#define MAILBOX_SET_CLOCK_RATE		0x00038002

#define MAILBOX_FB_ALLOC_BUFFER			0x00040001
#define MAILBOX_FB_SET_PHY_WH			0x00048003
#define MAILBOX_FB_SET_VIRTUAL_WH		0x00048004
#define MAILBOX_FB_SET_DEPTH			0x00048005
#define MAILBOX_FB_SET_RGB_ORDER		0x00048006
#define MAILBOX_FB_GET_PITCH			0x00040008
#define MAILBOX_FB_SET_VIRTUAL_OFFSET	0x00048009


#define MAILBOX_REQUEST_CODE        0x00000000
#define MAILBOX_REQUEST_SUCCEED     0x80000000
#define MAILBOX_REQUEST_FAILED      0x80000001
#define MAILBOX_TAG_REQUEST_CODE    0x00000000
#define MAILBOX_END_TAG             0x00000000

#define MAILBOX_PROPERTY_CHANNEL 8

#define MAILBOX_UART_CLOCK_ID	0x00000002


unsigned int mailbox_get_board_revision();
void mailbox_get_hardware_memory_info(
	unsigned int* base_address_in_bytes,
	unsigned int * size_in_bytes,
	bool is_vc);

void mailbox_get_clock_state(unsigned int clock_id,bool* is_on, bool* exist); 

unsigned int mailbox_get_clock_rate(unsigned int clock_id); 

void mailbox_set_clock_rate(unsigned int clock_id, unsigned int clock_rate); 

bool mailbox_init_framebuffer(FB_INFO* fb_info);

#endif