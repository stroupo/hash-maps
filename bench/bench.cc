#include<unordered_map>
#include<boost/unordered_map.hpp>
#include<boost/filesystem.hpp>
#include<memory>
#include<vector>
#include<chrono>
#include<random>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<algorithm>
#include<utility>
#include<cstdlib>
#include<hash_map.h>
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
typedef std::string KeyType;
typedef std::vector<Time> Timings;
typedef std::vector<std::pair<int, std::vector<Time>>> TimingResults;
std::string img_path = boost::filesystem::path full_path(boost::filesystem::current_path());

struct Range
{
	Range(int start, int stop, int step): start(start), stop(stop), step(step){};
	Range(int stop): start(0), stop(stop), step(1){};
	int start;
	int stop;
	int step;
};

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
Time measure(Function function, int repetitions = 1)
{
	auto start = std::chrono::high_resolution_clock::now();
	function();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    return std::chrono::duration<Time>(diff).count();
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
		// std::for_each(vec.begin(), vec.end(), 
			// [&vec, &uni, &rng](T& x){x = vec[uni(rng)];});
		std::transform(
				vec.begin(),
				vec.begin() + mults,
				vec.begin(),
				[&vec, &uni, &rng](T i){return vec[uni(rng)];});
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
template<typename T>
std::vector<T> range(Range &r)
{
	std::vector<T> vec;
	T i = r.start;
	while((r.step > 0) ? (i < r.stop) : (i > r.stop))
	{
		vec.push_back(i);
		i += r.step;
	}
	return vec;
}

template<typename T, class... HashMaps>
Timings seqInsert(int size, float non_unique, HashMaps... hms)
{
	std::vector<T> vec = makeVector<T>(size, non_unique);
	Timings tms{measure(mf_sequential_insertion(hms, vec))...};
	return tms;
}
template<typename KeyType>
TimingResults sequentialInsert(Range &r,
							   float unique,
						       bool verbose=true)
{
	std::vector<int> sizes = range<int>(r);
	TimingResults timing_results;
	for(int size : sizes)
	{
		// std::unordered_map<KeyType, int, std::hash<KeyType>> stl_hm;
		stroupo::hash_map<KeyType, int> stroupo_hm;
		std::unordered_map<KeyType, int> stl_hm;
		boost::unordered_map<KeyType, int> boost_hm;
		// boost::unordered_map<KeyType, int, std::hash<KeyType>> boost_hm;
		Timings timings = seqInsert<KeyType>(size, unique,
											 boost_hm,
											 stl_hm,
											 stroupo_hm);
		if(verbose)
		{
			std::cout << size;
			for(auto t : timings) std::cout << "\t" << t;
			std::cout << "\n";
		}
		timing_results.push_back({size, timings});
	}
	return timing_results;
}

std::string trToPylist(TimingResults &trs)
{
	std::string str = "[";
	for(auto tr: trs)
	{
		str.append("[");
		str.append(std::to_string(tr.first));
		for(auto r : tr.second)
		{
			str.append(",");
			str.append(std::to_string(r));
		}
		str.append("],\n");
	}
	str.append("]");
	return str;
}
std::string trToPython(TimingResults &trs,
				std::string title,
				std::string filename,
				std::vector<std::string> names,
				std::string xlabel="input size",
				std::string ylabel="time / s")
{
	std::string code;
	code.append("import matplotlib.pyplot as plt\n");
	code.append("xys=" + trToPylist(trs) + "\n");
	code.append("title='" + title +"'\n");
	code.append("filename='" + filename + "'\n");
	code.append("names=[");
	for(auto name : names) code.append("'" + name + "'," );
	code.append("]\n");
	code.append("xlabel='"+xlabel+"'\n");
	code.append("ylabel='"+ylabel+"'\n");
	code.append("xys = list(map(list, zip(*xys)))\n");
	code.append("xs = xys[0]\n");
	code.append("for ys, name in zip(xys[1:], names):\n");
	code.append("\tplt.plot(xs, ys, label=name)\n");
	code.append("plt.xlabel(xlabel)\n");
	code.append("plt.ylabel(ylabel)\n");
	code.append("plt.title(title)\n");
	code.append("plt.legend()\n");
	code.append("plt.savefig(filename)\n");
	code = "\"" + code + "\""; 
	// return "\"print('Hello World!')\"";
	return code;
}

/*
Get Memory Footprint of all the operations
Realistic use-case, insert, lookup, delete
Lookup existing String
Lookup non-existing int
Erase existing int
Erase non-existing String

*/
template<typename KeyType>
void benchSequentialInsertions(Range &r, std::string keytype, float unique = 0.0f)
{
	TimingResults trs = sequentialInsert<KeyType>(r, unique);
	makeUniqueString mus;
	std::string code = trToPython(
		trs,
		"Sequential Inserts - Unique: "+ std::to_string(unique)  +"  - " + keytype,
		img_path + mus(),
		{"BOOST", "STL", "STROUPO"});
	std::system(("python -c " + code).c_str());
}

template<typename KeyType>
void no_overhead_si()
{
	Range r{100'000, 2'500'000, 100'000};
	std::vector<int> sizes = range<int>(r);
	for(int size : sizes)
	{
		stroupo::hash_map<KeyType, int> stroupo_hm;
		std::unordered_map<KeyType, int> stl_hm;
		boost::unordered_map<KeyType, int> boost_hm;

		std::vector<KeyType> vec = makeVector<KeyType>(size,
													   0.0f);

		auto start = std::chrono::high_resolution_clock::now();
		for(KeyType k : vec)
		{
			boost_hm.insert({k, 0});
		}
    	auto end = std::chrono::high_resolution_clock::now();
    	auto diff = end - start;
    	Time boost_time = std::chrono::duration<Time>(diff).count();

    	start = std::chrono::high_resolution_clock::now();
		for(KeyType k : vec)
		{
			stl_hm.insert({k, 0});
		}
    	end = std::chrono::high_resolution_clock::now();
    	diff = end - start;
    	Time stl_time = std::chrono::duration<Time>(diff).count();

    	start = std::chrono::high_resolution_clock::now();
		for(KeyType k : vec)
		{
			stroupo_hm.insert({k, 0});
		}
    	end = std::chrono::high_resolution_clock::now();
    	diff = end - start;
    	Time stroupo_time = std::chrono::duration<Time>(diff).count();
    	std::cout << size << "\t" << boost_time << "\t" << stl_time << "\t" << stroupo_time << "\n";
	}
}
template<typename KeyType>
void no_overhead_lookups()
{
	Range r{200'000, 1'000'000, 200'000};
	std::vector<int> sizes = range<int>(r);
	for(int size : sizes)
	{
		stroupo::hash_map<KeyType, int> stroupo_hm;
		std::unordered_map<KeyType, int> stl_hm;
		boost::unordered_map<KeyType, int> boost_hm;

		std::vector<KeyType> vec = makeVector<KeyType>(size,
													   0.0f);
		int n_missing{1000};
		int n_contained{1000};
		int n_total{n_missing + n_contained};
		std::vector<KeyType> lookups(n_total);
		std::copy(vec.end() - n_total,
				  vec.end(),
				  lookups.begin());
		vec.erase(vec.end() - n_missing,
				  vec.end());

		for(KeyType k : vec)
		{
			boost_hm.insert({k, 0});
			stl_hm.insert({k, 0});
			stroupo_hm.insert({k, 0});
		}
		auto start = std::chrono::high_resolution_clock::now();
		for(KeyType k : lookups)
		{
			boost_hm.find(k);
		}
    	auto end = std::chrono::high_resolution_clock::now();
    	auto diff = end - start;
    	Time boost_time = std::chrono::duration<Time>(diff).count();

    	start = std::chrono::high_resolution_clock::now();
    	for(KeyType k : lookups)
    	{
			stl_hm.find(k);
    	}
    	end = std::chrono::high_resolution_clock::now();
    	diff = end - start;
    	Time stl_time = std::chrono::duration<Time>(diff).count();

    	start = std::chrono::high_resolution_clock::now();
		for(KeyType k : lookups)
		{
			stroupo_hm.find(k);
		}
    	end = std::chrono::high_resolution_clock::now();
    	diff = end - start;
    	Time stroupo_time = std::chrono::duration<Time>(diff).count();
    	std::cout << size << "\t" << boost_time << "\t" << stl_time << "\t" << stroupo_time << "\n";
    	// std::cout << size << "\t" << boost_time << "\t" << stl_time << "\n";
	}
}
int main()
{
	// no_overhead_lookups<std::string>();
	// Range r{(int) 1e5, (int) 1e7, (int) 1e5};
	// Range r{100'000, 2'000'000, 100'000};
	// benchSequentialInsertions<std::string>(r, "str", 0.0f);
	// std::cout << "D\n";
	// benchSequentialInsertions<std::string>(r, "str", 0.3f);
	// std::cout << "D\n";
	// Range r2{100'000, 1'300'000, 100'000};
	// benchSequentialInsertions<int>(r2, "int", 0.0f);
	// std::cout << "D\n";
	// benchSequentialInsertions<int>(r2, "int", 0.03f);
	// std::vector<int> v =  makeVector<int>(10, 0.2);
	// for(int i: v)std::cout << i << std::endl;
	// std::vector<int> a{1,2,3,4,5};
	// for(auto i: a) std::cout << i << " ";
	// std::vector<int> b(3);
	// std::cout << "\n";
	// std::copy(a.end() - 3,
	// 		  a.end(),
	// 		  b.begin());
	// for(auto i: b) std::cout << i << " ";
	// std::cout << "\n";
	// a.erase(a.end() - 3, a.end());
	// for(auto i: a) std::cout << i << " ";

}