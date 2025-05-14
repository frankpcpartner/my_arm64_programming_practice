#ifndef _UTIL_H
#define _UTIL_H

#include "core.h"

bool strcmp(const char* str1, const char* str2);

void llu_to_hex_str(unsigned long long llu, char* str,bool upcase,bool add_leading_zero);

void llu_to_str(unsigned long long llu, char* str);

void lld_to_str(long long lld, char* str);

void double_to_str(double ul,int precision, char* str);

void reverse_str(char* str, int len);

void printf(const char* fmt,...);

void memset(void* dst, int c, size_t size);

#endif