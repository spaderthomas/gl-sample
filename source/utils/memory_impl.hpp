void init_temporary_storage() {
	arr_init(&temporary_storage.memory_block, TemporaryStorage::SIZE);
}

void update_temporary_storage() {
	temporary_storage.clear();
}

void TemporaryStorage::clear() {
	arr_clear(&this->memory_block);
}

char* TemporaryStorage::alloc_path() {
	return (char*)this->alloc(MAX_PATH_LEN);
}

void* TemporaryStorage::alloc(int32 size) {
	fm_assert(this->memory_block.size + size <= this->memory_block.capacity);
	
	void* data = arr_back(&this->memory_block);
	this->memory_block.size += size;
	
	return data;
}

template<typename T>
Array<T> TemporaryStorage::alloc_array(int32 size) {
	Array<T> array;
	array.data = (T*)alloc(size * sizeof(T));
	array.size = 0;
	array.capacity = size;

	return array;
}

template<typename T>
T* TemporaryStorage::alloc() {
	return (T*) alloc(sizeof(T));
}
