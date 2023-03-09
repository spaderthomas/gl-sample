typedef const char* tstring;

struct TemporaryStorage {
	static constexpr int32 SIZE = 200 * 1024 * 1024;
	
	Array<char> memory_block;

	template<typename T>
	T* alloc();
	
	void* alloc(int32 size);

	template<typename T>
	Array<T> alloc_array(int32 size);

	char* alloc_path();
	
	void clear();
};
TemporaryStorage temporary_storage;

void init_temporary_storage();
void update_temporary_storage();
