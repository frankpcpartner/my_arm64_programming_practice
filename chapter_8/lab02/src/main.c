#include <stdio.h>



extern long long get_max(long long a, long long b);


int main(void)
{	
	long long a = 10, b = 100;

	long long ret = get_max(a, b);

	printf("a:%lld,b:%lld,max:%lld\r\n",a,b,ret);

	a = 902;
	ret = get_max(a, b);

	printf("a:%lld,b:%lld,max:%lld\r\n",a,b,ret);


	a = -102;
	ret = get_max(a, b);

	printf("a:%lld,b:%lld,max:%lld\r\n",a,b,ret);


	b = -10;
	ret = get_max(a, b);

	printf("a:%lld,b:%lld,max:%lld\r\n",a,b,ret);

	return 0;
}