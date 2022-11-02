#include <stdio.h>

#pragma pack(16)
struct align
{
	char a[18];
	double b;
	char c;
	int d;
	short e;
};

template <int N>
int Fibonacci()
{
return Fibonacci<N-1>()+Fibonacci<N-2>();
}

template<> int Fibonacci<0>() { return 1;}
template<> int Fibonacci<1>() { return 1;}

int main()
{
	int a = -1;
	unsigned b = 1;
	if ( a > b)
		printf("a>b, a = %d, b = %u\n",a,b);
	else
		printf("a<=b, a = %d, b = %u\n",a,b);
	printf("sizeof align : %lu\n",sizeof(align));
	printf("Fibonacci 3 = %d\n", Fibonacci<3>());
	return 0;
}
