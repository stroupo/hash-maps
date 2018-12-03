#include <iostream>
#include <tuple>
#include <vector>

template<typename T>
double sum(T t)
{
	return t;
}
template<typename T, typename... Rest>
double sum(T t, Rest... rest)
{
	return t + sum(rest...);
}

template<typename T>
void out(T i)
{
	std::cout << i << std::endl;
}
template<typename T>
int ri(T t)
{
	return 42;
}

template<typename... Rest>
void dos(Rest... rest)
{	
	// std::tuple<Rest...> argsTuple{rest...};
	std::vector<Rest...> argsTuple{rest...};
}

int main()
{
	dos(1,2,3);
}