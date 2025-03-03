#include "reloc_img.h"

extern unsigned char _rel_dyn_size;

void relocate_code(long long offset,long long rel_dyn_start)
{
    //Fix rel section
    long long rel_size =  (long long)&_rel_dyn_size / sizeof(REL_INFO);
    PREL_INFO rel_info = (PREL_INFO)(rel_dyn_start);
    
    for(int i = 0; i < rel_size; i++)
    {
        unsigned long long type = rel_info[i].info &0xffffffff;
        if(type == R_AARCH64_RELATIVE)
        {
            rel_info[i].offset += offset;
            rel_info[i].addend += offset;
            long long* fix = (long long*)(rel_info[i].offset);
            (*fix) = rel_info[i].addend;   
        }
    }    
}