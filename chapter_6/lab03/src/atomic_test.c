

extern unsigned int my_atomic_write(unsigned int data);

int main()
{
	unsigned int p1 = my_atomic_write(0x34);

	printf("Test\r\n");

	return 0;
}