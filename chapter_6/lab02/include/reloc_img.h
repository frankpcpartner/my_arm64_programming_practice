#ifndef _RELOC_IMG_H
#define _RELOC_IMG_H

#define R_AARCH64_RELATIVE 1027

typedef struct _rel_info
{
	unsigned long long 	offset;
	unsigned long long 	info;
	long long 			addend;
}REL_INFO,*PREL_INFO;


#endif