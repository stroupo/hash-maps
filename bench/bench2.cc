#include<unordered_map>
#include<boost/unordered_map.hpp>
#include<memory>
#include<vector>
#include<chrono>
#include<random>
#include<algorithm>
#include<iostream>
#include<string>
#include<cmath>
#include<algorithm>

// boost_hm::erase
// stl_hm::erase
// google_gm::delete

// template <typename HashMap>
// void measure_erase(HashMap& map, ...){
// 	clock::now();
// 	map.erase(random);
// 	hash_map_traits<HashMap>::erase(map,...);
// 	clock::now();
// }

template<typename HashMap>
auto mf_sequential_insertion_unique(
	HashMap &hm,
	std::vector<typename HashMap::key_type> &keyvec)
{
	return [&hm, &keyvec](){
		for(auto key : keyvec)
		{
			hm.insert({key, 0});
		}
	};
}

template <typename Function>
float measure(Function function)
{
	auto start = std::chrono::high_resolution_clock::now();
	function();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    const float elapsed = std::chrono::duration<float>(diff).count();
	return elapsed;
}

struct makeUniqueString
{
	makeUniqueString(
		int length=32,
		int count=0) : length(length), count(count), rng(rd()){}
	int length;
	int count;
	std::string str{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
	std::random_device rd;
	std::mt19937 rng;
	std::string operator()()
	{
		std::shuffle(str.begin(), str.end(), rng);
		return std::to_string(count++).append(str).substr(0, 32);
	}
};
template<typename T>
std::vector<T> makeUniqueVector(int size);

template<>
std::vector<int> makeUniqueVector<int>(int size)
{
	std::vector<int> vec(size);
	std::iota(vec.begin(), vec.end(), 0);
	return vec;
}
template<>
std::vector<std::string> makeUniqueVector<std::string>(int size)
{
	std::vector<std::string> vec(size);
	makeUniqueString mus;
	std::generate(vec.begin(), vec.end(), [&](){ return mus(); });
	return vec;
}
template<typename T> 
std::vector<T> makeVector(int size,
	  					  float non_unique = 0.0f)
{
	std::vector<T> vec = makeUniqueVector<T>(size);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, vec.size() - 1);

	if(non_unique > 0.0f)
	{
		int mults = (int) std::floor(non_unique * size);
		std::transform(
				vec.begin(),
				vec.begin() + mults,
				vec.begin(),
				[&vec, &uni, &rng](T i){return vec[uni(rng)];});
	}
	std::shuffle(vec.begin(), vec.end(), rng);
	return vec;

}
int main()
{
	// typedef std::string T;
	typedef std::string T;
	std::vector<T> vec = makeVector<T>((int) 1E4);
	
	std::unordered_map<T, int, std::hash<T>> stl_hm;
	auto function = mf_sequential_insertion_unique(stl_hm, vec);
	float elapsed = measure(function);
	std::cout << elapsed << std::endl;

	boost::unordered_map<T, int, std::hash<T>> boost_hm;
	elapsed = measure(mf_sequential_insertion_unique(boost_hm, vec));
	std::cout << elapsed << std::endl;

	// int size=10;
	// std::vector<std::string> vec = makeVector<std::string>(size);
	// for(int i=0; i<size; i++)
	// {
	// 	std::cout << vec[i] << std::endl;
	// }
}