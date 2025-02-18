#include "mailbox.h"
#include "util.h"

/* mailbox message buffer */
volatile unsigned int  __attribute__((aligned(16))) mailbox[36];


static unsigned int mailbox_read(byte channel)
{
	// Loop until we receive something from the requested channel
	for (;;)
	{
		while ((get(MAILBOX_STATUS) & MAILBOX_EMPTY) != 0)
		{
			asm volatile("nop");
		}
		// Read the data
		unsigned int data = get(MAILBOX_READ);
		byte readChannel = data & 0xF;
		data >>= 4;
		// Return it straight away if it's for the requested channel
		if (readChannel == channel)
			return data;
	}
}


static void mailbox_write(byte channel,unsigned int data)
{
	while ((get(MAILBOX_STATUS) & MAILBOX_FULL) != 0)
	{
		asm volatile("nop");
	}
	// Write the value to the requested channel
	set(MAILBOX_WRITE,(data << 4) | channel);
}


static void mailbox_call(volatile unsigned int* buffer)
{
	unsigned int data_addr = ((unsigned long long)buffer & (~0xf)) >> 4;

	mailbox_write(MAILBOX_PROPERTY_CHANNEL,data_addr);
	mailbox_read(MAILBOX_PROPERTY_CHANNEL);
}

unsigned int mailbox_get_board_revision()
{
	mailbox[0] = 7 * 4; // buffer size in bytes
	mailbox[1] = MAILBOX_REQUEST_CODE;

	// tags begin
	mailbox[2] = MAILBOX_GET_BOARD_REVISION; // tag identifier
	mailbox[3] = 4; // maximum of request and response value buffer's length.
	mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
	mailbox[5] = 0; // value buffer
	// tags end
	mailbox[6] = MAILBOX_END_TAG;

	mailbox_call(mailbox); // message passing procedure call, you should implement it following the 6 steps provided above.
	return mailbox[5];
}

void mailbox_get_hardware_memory_info(
	unsigned int* base_address_in_bytes,
	unsigned int * size_in_bytes,
	bool is_vc)
{
	mailbox[0] = 8 * 4; // buffer size in bytes
	mailbox[1] = MAILBOX_REQUEST_CODE;

	// tags begin
	mailbox[2] = is_vc ? MAILBOX_GET_VC_MEMORY : MAILBOX_GET_ARM_MEMORY; // tag identifier
	mailbox[3] = 8; // maximum of request and response value buffer's length.
	mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
	mailbox[5] = 0; // value buffer
	mailbox[6] = 0; // value buffer
	// tags end
	mailbox[7] = MAILBOX_END_TAG;

	mailbox_call(mailbox); // message passing procedure call, you should implement it following the 6 steps provided above.

	(*base_address_in_bytes) = mailbox[5];
	(*size_in_bytes) = mailbox[6];
}

void mailbox_get_clock_state(unsigned int clock_id,bool* is_on, bool* exist)
{
	mailbox[0] = 8 * 4; // buffer size in bytes
	mailbox[1] = MAILBOX_REQUEST_CODE;

	// tags begin
	mailbox[2] = MAILBOX_GET_CLOCK_STATE; // tag identifier
	mailbox[3] = 8; // maximum of request and response value buffer's length.
	mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
	mailbox[5] = clock_id; // value buffer
	mailbox[6] = 0; // value buffer
	// tags end
	mailbox[7] = MAILBOX_END_TAG;

	mailbox_call(mailbox); // message passing procedure call, you should implement it following the 6 steps provided above.

	(*is_on) = mailbox[6] & 0x1;
	(*exist) = mailbox[6] & 0x2;
}

unsigned int mailbox_get_clock_rate(unsigned int clock_id) 
{
	mailbox[0] = 8 * 4; // buffer size in bytes
	mailbox[1] = MAILBOX_REQUEST_CODE;

	// tags begin
	mailbox[2] = MAILBOX_GET_CLOCK_RATE; // tag identifier
	mailbox[3] = 8; // maximum of request and response value buffer's length.
	mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
	mailbox[5] = clock_id; // value buffer
	mailbox[6] = 0; // value buffer
	// tags end
	mailbox[7] = MAILBOX_END_TAG;

	mailbox_call(mailbox); // message passing procedure call, you should implement it following the 6 steps provided above.

	return mailbox[6];
} 

void mailbox_set_clock_rate(unsigned int clock_id, unsigned int clock_rate)
{
	mailbox[0] = 9 * 4; // buffer size in bytes
	mailbox[1] = MAILBOX_REQUEST_CODE;

	// tags begin
	mailbox[2] = MAILBOX_SET_CLOCK_RATE; // tag identifier
	mailbox[3] = 12; // maximum of request and response value buffer's length.
	mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
	mailbox[5] = clock_id; // value buffer
	mailbox[6] = clock_rate; // value buffer
	mailbox[7] = 0;
	// tags end
	mailbox[8] = MAILBOX_END_TAG;

	mailbox_call(mailbox); // message passing procedure call, you should implement it following the 6 steps provided above.	
}

bool mailbox_init_framebuffer(FB_INFO* fb_info)
{
	mailbox[0] = 35*4;
    mailbox[1] = MAILBOX_REQUEST_CODE;

    mailbox[2] = MAILBOX_FB_SET_PHY_WH;  //set phy wh
    mailbox[3] = 8;
    mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[5] = fb_info->width;         //FrameBufferInfo.width
    mailbox[6] = fb_info->height;          //FrameBufferInfo.height

    mailbox[7] = MAILBOX_FB_SET_VIRTUAL_WH;  //set virt wh
    mailbox[8] = 8;
    mailbox[9] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[10] = fb_info->width;        //FrameBufferInfo.virtual_width
    mailbox[11] = fb_info->height;         //FrameBufferInfo.virtual_height

    mailbox[12] = MAILBOX_FB_SET_VIRTUAL_OFFSET; //set virt offset
    mailbox[13] = 8;
    mailbox[14] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[15] = 0;           //FrameBufferInfo.x_offset
    mailbox[16] = 0;           //FrameBufferInfo.y.offset

    mailbox[17] = MAILBOX_FB_SET_DEPTH; //set depth
    mailbox[18] = 4;
    mailbox[19] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[20] = fb_info->depth;          //FrameBufferInfo.depth

    mailbox[21] = MAILBOX_FB_SET_RGB_ORDER; //set pixel order
    mailbox[22] = 4;
    mailbox[23] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[24] = fb_info->is_rgb;           //RGB, not BGR preferably

    mailbox[25] = MAILBOX_FB_ALLOC_BUFFER; //get framebuffer, gets alignment on request
    mailbox[26] = 8;
    mailbox[27] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[28] = fb_info->alignment;        //FrameBufferInfo.pointer
    mailbox[29] = 0;           //FrameBufferInfo.size

    mailbox[30] = MAILBOX_FB_GET_PITCH; //get pitch
    mailbox[31] = 4;
    mailbox[32] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[33] = 0;           //FrameBufferInfo.pitch

    mailbox[34] = MAILBOX_END_TAG;

    mailbox_call(mailbox);

    if(mailbox[20] == 32 && mailbox[28] != 0)
    {
    	mailbox[28] &= 0x3FFFFFFF;//convert GPU address to ARM address
    	fb_info->width = mailbox[5];          //get actual physical width
        fb_info->height = mailbox[6];         //get actual physical height
        fb_info->pitch = mailbox[33];         //get number of bytes per line
        fb_info->is_rgb = mailbox[24];         //get the actual channel order
        fb_info->buffer = (void*)((unsigned long)mailbox[28]);
		fb_info->size = mailbox[29];
        return true;
    }

    return false;
}