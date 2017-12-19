#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const * argv [])
{
	int a = 5;
	printf("This is a very simple test program.\n");
	int b = 7;
	printf("It only prints out 2+2=%d\n", 2+2);
	int c = a + b / 2;
	printf("(a + b) / 2 = %d", c);

	while (c < 100)
	{
		c += c;
	}

	return c;
}
