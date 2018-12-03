#include<iostream>

template<typename T>
double fv(T i)
{
	return 1.0;
}

void pa(const double* x_array, const int size)
{
	for(int i=0; i< size;++i)
	{
		std::cout << x_array[i] << std::endl;
	}
}

template<class... Args>
void h(Args... args)
{
  double x_array[] = { fv(args)... };

  pa(x_array, sizeof...(Args));
}

int main()
{
	h(1,2,3,4);
}