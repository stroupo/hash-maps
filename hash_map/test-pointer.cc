#include <iostream>
int main()
{
	int a[] = {1,2,3};
	int *ap[3];
	ap[0] = &a[0];
	ap[1] = &a[1];
	ap[2] = &a[2];

	std::cout << *ap[2] << std::endl;
}