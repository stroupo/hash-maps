#include<unordered_map>
#include<boost/unordered_map.hpp>
#include<memory>
#include<vector>
#include<chrono>
#include<random>
#include<algorithm>
#include<iostream>
#include <fstream>
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

typedef double Time;

template<typename HashMap>
auto mf_sequential_insertion(
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
float measure(Function function, int repetitions = 10)
{
	float elapsed_total = 0;
	for(int i=0; i<repetitions; i++)
	{
	    elapsed_total += time_single_execution(function);
	}
	return elapsed_total / repetitions;
}
template <typename Funtion>
float time_single_execution(Funtion function)
{
	auto start = std::chrono::high_resolution_clock::now();
	function();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    return std::chrono::duration<float>(diff).count();
}
struct makeUniqueString
{
	makeUniqueString(
		int length=32,
		int count=0) : length(length), count(count) {}
	int length;
	int count;
	std::string str{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
	// std::random_device rd;
	std::mt19937 rng{std::random_device{}()};
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
		std::for_each(vec.begin(), vec.end(), 
			[&vec, &uni, &rng](T& x){x = vec[uni(rng)];});
	}
	std::shuffle(vec.begin(), vec.end(), rng);
	return vec;

}
template<typename X, typename Y>
void writeResultToFile(std::string filename,
					   const std::vector<std::pair<X, Y>> &vec,
					   char delimiter = '\n',
					   char seperator = ',')
{
	std::ofstream outfile;
	outfile.open(filename);
	for(auto pair : vec)
	{
		outfile << pair.first << seperator << pair.second << delimiter;
	}
	outfile.close();
}
template<typename T>
std::vector<T> range(T start, T stop, T step)
{
	std::vector<T> vec;
	T i = start;
	while((step > 0) ? (i < stop) : (i > stop))
	{
		vec.push_back(i);
		i += step;
	}
	return vec;
}
template<typename KeyType>
void sequentialInsert()
{
	std::vector<int> sizes = range<int>((int) 1E4,
	                                    (int) 1E5,
	                                    (int) 1E4);
	std::vector<std::pair<int, float>> results_stl;
	std::vector<std::pair<int, float>> results_boost;
	for (int size : sizes)
	{
		std::vector<KeyType> vec = makeVector<KeyType>((int) size);
		float elapsed = 0.0f;
		
		std::unordered_map<KeyType, int, std::hash<KeyType>> stl_hm;
		elapsed = measure(mf_sequential_insertion(stl_hm, vec));
		results_stl.push_back({size, elapsed});
		std::cout << "STL: " << size << ": " << elapsed << std::endl;


		boost::unordered_map<KeyType, int, std::hash<KeyType>> boost_hm;
		elapsed = measure(mf_sequential_insertion(boost_hm, vec));
		results_boost.push_back({size, elapsed});
		std::cout << "BOOST:" << size << ": " << elapsed << std::endl;
	}
	writeResultToFile("seq_insert_stl.csv", results_stl);
	writeResultToFile("seq_insert_boost.csv", results_boost);
	std::cout << sizes.size() << std::endl;
}
// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}

template<typename T, class... HashMaps>
void seqInsert(int size, HashMaps... hms)
{
	std::vector<T> vec = makeVector<T>(size);
	std::vector<float> measures = {measure(mf_sequential_insertion(hms, vec))...};
	for(auto m : measures) std::cout << m << std::endl;
	std::cout << "can call \n" << std::endl;
}

/*
Get Memory Footprint of all the operations
Realistic use-case, insert, lookup, delete
Sequential insert unique int
Sequential insert unique string
Sequential insert non-unique int
Sequential insert non-unique string
Lookup existing String
Lookup non-existing int
Erase existing int
Erase non-existing String

*/
int main()
{
	typedef std::string T;
	std::vector<T> vec = makeVector<T>((int) 1E3);
	
	std::unordered_map<T, int, std::hash<T>> stl_hm;
	auto function = mf_sequential_insertion(stl_hm, vec);
	// float elapsed = measure(function);
	// std::cout << elapsed << std::endl;

	boost::unordered_map<T, int, std::hash<T>> boost_hm;
	// elapsed = measure(mf_sequential_insertion(boost_hm, vec));
	// std::cout << elapsed << std::endl;


	boost_hm.insert({"bla", 1});
	stl_hm.insert({"bla", 1});

	seqInsert<T>(100, boost_hm, stl_hm);
	// seqInsert(boost_hm);
	// print_container(boost_hm, stl_hm);
	// sequentialInsert<std::string>();
	// for(auto sz : sz_vecs)
	// {
	// 	std::vector<T> vec = makeVector<T>(sz);

	// 	auto function = mf_sequential_insertion(stl_hm, vec);
	// 	float elapsed = measure(function);
	// 	std::cout << "STL: " << elapsed << std::endl;

	// 	boost::unordered_map<T, int, std::hash<T>> boost_hm;
	// 	elapsed = measure(mf_sequential_insertion(boost_hm, vec));
	// 	std::cout << "BST: " << elapsed << std::endl;
	// }

	// int size=10;
	// std::vector<std::string> vec = makeVector<std::string>(size);
	// for(int i=0; i<size; i++)
	// {
	// 	std::cout << vec[i] << std::endl;
	// }
}