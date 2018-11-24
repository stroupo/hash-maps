#include<unordered_map>
#include<memory>
#include<vector>
#include<chrono>
#include<random>

boost_hm::erase
stl_hm::erase
google_gm::delete

template <typename HashMap>
void measure_erase(HashMap& map, ...){
	clock::now();
	map.erase(random);
	hash_map_traits<HashMap>::erase(map,...);
	clock::now();
}


template<typename Key, typename Value>
class HashMap
{
public:
	virtual void insert(Key k, Value v) = 0;
	virtual int getSize() = 0;
	virtual int getMaxSize() = 0;
};

template<typename Key, typename Value>
class STLHashMap : public HashMap<Key, Value> {
public:
	STLHashMap(){
		ptr_hm = std::make_unique<std::unordered_map<Key,Value>>();
	}
	inline void insert(Key k, Value v) 
	{
		ptr_hm->insert({k, v});
	}
	inline int getSize()
	{
		return ptr_hm->size();
		// return 0;
	}
	inline int getMaxSize()
	{
		return ptr_hm->max_size();
	}
private:
	std::unique_ptr<std::unordered_map<Key, Value>> ptr_hm;
	std::unordered_map<Key, Value > um{};
};


template<typename T>
std::vector<T> makeShuffledVector(int size)
{
	std::vector<T> vec(size);
	std::iota(vec.begin(), vec.end(), 0);
	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(vec), std::end(vec), rng);
	return vec;
}

// template<typneme T>
// void shuffle(std::vector<T> vec)
// {

// }

int main()
{
	STLHashMap<int, int> stl_hm;
	std::vector<int> = makeShuffledVector(10);
}