#include<iostream>
#include<vector>

template<typename T>
double fv(T i)
{
	return 1.0;
}

void pa(std::vector<double> &vec, const int size)
{
	for(int i=0; i< size;++i)
	{
		std::cout << vec[i] << std::endl;
	}
}

template<class... Args>
void h(Args... args)
{
  std::vector<double> vec = { fv(args)... };

  pa(vec, sizeof...(Args));
}

int main()
{
	h(1,2,3,4);
}