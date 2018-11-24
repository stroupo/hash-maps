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


std::vector<int> makeShuffledVectorInt(int size)
{
	std::vector<int> vec(size);
	std::iota(vec.begin(), vec.end(), 0);
	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(vec), std::end(vec), rng);
	return vec;
}

std::vector<std::string> makeShuffledVectorString(int size)
{
	std::vector<std::string> vec(size);
	return vec;
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
	  					  bool shuffle=true,
	  					  bool unique=true,
	  					  float share=0.7)
{
	return makeUniqueVector<T>(size);
}
int main()
{
	std::vector<int> vec = makeVector<int>((int) 1E4);
	std::vector<std::string> vec2 = makeVector<std::string>((int) 1E4);
	
	std::unordered_map<int, int> stl_hm;
	auto function = mf_sequential_insertion_unique(stl_hm, vec);
	float elapsed = measure(function);
	std::cout << elapsed << std::endl;

	boost::unordered_map<int, int> boost_hm;
	elapsed = measure(mf_sequential_insertion_unique(boost_hm, vec));
	std::cout << elapsed << std::endl;

	// std::vector<int> vi = makeUniqueVector<int>(10);
	std::vector<std::string> vs = makeUniqueVector<std::string>(10);

	makeUniqueString mus;
	makeUniqueString mus2;
	for(int i=0;i<10;i++)
	{
		std::cout << vs[i] << std::endl;
		// std::cout << mus() << std::endl;
		// std::cout << mus2() << std::endl;
	}
}