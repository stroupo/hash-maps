# Hash Maps

Fr 16:
- templatization

- stl interface
- How to test?
- Performance benchmarks?
	- with stl and boost	
	- speed
	- memory consumption
- templatization
- stl interface
- robin hood hashing
- std::unordered_map

Resources:
- https://probablydance.com/2014/05/03/i-wrote-a-fast-hash-table/
- http://bannalia.blogspot.com/2014/01/a-better-hash-table-clang.html
- https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
- https://stackoverflow.com/questions/34563574/how-to-build-hash-function-for-various-template-types




Idea:
key:value
index = hash(key)
store: key:value

provide hash function for each type

value
int
