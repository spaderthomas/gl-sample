void memfill(void* dst, int32 size, void* pattern, int32 pattern_size) {
	char* cdst = (char*)dst;
	char* cptn = (char*)pattern;
	int i = 0;

	while (true) {
		if (i + pattern_size > size) return;
		memcpy(cdst + i, cptn, pattern_size);
		i += pattern_size;
	}
}

template<typename T>
struct Array {
	int32 size      = 0;
	int32 capacity  = 0;
	T* data         = nullptr;

	T* operator[](uint32 index) { fm_assert(index < size); return data + index; }
};

template<typename T>
void arr_init(Array<T>* array, int32 capacity) {
	array->size = 0;
	array->capacity = capacity;
	array->data = (T*)calloc(capacity, sizeof(T));
}

template<typename T>
void arr_init(Array<T>* array, int32 capacity, T fill) {
	array->size = 0;
	array->capacity = capacity;
	array->data = (T*)calloc(capacity, sizeof(T));
	arr_fill(array, T());
}

template<typename T>
void arr_clear(Array<T>* array) {
	memset(array->data, 0, array->size * sizeof(T));
	array->size = 0;
}

template<typename T>
void arr_fastclear(Array<T>* array) {
	array->size = 0;
}

template<typename T>
void arr_fill(Array<T>* array, T element) {
	memfill(array->data, arr_bytes(array), &element, sizeof(T));
	array->size = array->capacity; 
}

template<typename T>
void arr_fill(Array<T>* array, T element, int32 count) {
	memfill(array->data, count * sizeof(T), &element, sizeof(T));
	array->size = count;
}

template<typename T>
void arr_fill(Array<T>* array, int32 offset, int32 count, T element) {
	memfill(array->data + offset, count * sizeof(T), &element, sizeof(T));
	array->size = count;
	// You're on your own as far as the size here
}

// Use case: You declare some array on the stack. It's empty, and you only want to modify its elements
// using an Array. Call this to wrap it in an empty Array of the correct capacity.
template<typename T>
Array<T> arr_stack(T* data, int32 capacity) {
	fm_assert(data);

	Array<T> array;
	array.size = 0;
	array.capacity = capacity;
	array.data = data;
	return array;
}

template<typename T, int32 N>
Array<T> arr_stack(T (&c_array)[N]) {
	Array<T> array;
	array.size = 0;
	array.capacity = N;
	array.data = c_array;
	return array;
}

// Use case: You have some contiguous data filled out somewhere (maybe in another Array, maybe in a C
// array). You want to RW a subarray using Array functions. Call this to wrap the subarray. 
template<typename T>
Array<T> arr_slice(Array<T>* array, int32 index, int32 size) {
	fm_assert(index >= 0);
	fm_assert(index + size <= array->capacity);
	
	Array<T> view;
	view.size = size;
	view.capacity = size;
	view.data = array->data + index;
	
	return view;
}

template<typename T>
Array<T> arr_slice(T* data, int32 size) {
	Array<T> arr;
	arr.size = size;
	arr.capacity = size;
	arr.data = data;
	
	return arr; 
}

template<typename T, int32 N>
Array<T> arr_slice(T (&data)[N]) {
	Array<T> array;
	array.size = N;
	array.capacity = N;
	array.data = data;
	return array;
}

template<typename T>
int32 arr_indexof(Array<T>* array, T* element) {
	int32 index = element - array->data;
	fm_assert(index >= 0);
	fm_assert(index < array->size);
	return index;
}

template<typename T>
T* arr_at(Array<T>* array, int32 index) {
	return (*array)[index];
}


////////////////
// ARRAY PUSH //
////////////////
template<typename T>
T* arr_push(Array<T>* array, const T* data, int32 count) {
	int32 remaining = array->capacity - array->size;
	fm_assert(remaining >= count && "Insufficient space remaining!");

	if (remaining < count) return nullptr;
	
	memcpy(array->data + array->size, data, sizeof(T) * count);
	T* out = array->data + array->size;
	array->size += count;
	return out;
}

template<typename T>
T* arr_push(Array<T>* array, T* data, int32 count) {
	int32 remaining = array->capacity - array->size;
	fm_assert(remaining >= count && "Insufficient space remaining!");
	
	memcpy(array->data + array->size, data, sizeof(T) * count);
	T* out = array->data + array->size;
	array->size += count;
	return out;
}

template<typename T>
T* arr_push(Array<T>* array, T element, int32 count) {
	auto back = array->data + array->size;
	memfill(back, count * sizeof(T), &element, sizeof(T));
	array->size += count;
	return back;
}

template<typename T>
T* arr_push(Array<T>* array, T* data) {
	fm_assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = *data;
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}

template<typename T>
T* arr_push(Array<T>* array, T data) {
	fm_assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = data;
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}

template<typename T>
T* arr_push(Array<T>* array) {
	fm_assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = T();
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}


///////////////
// ARRAY POP //
///////////////
template<typename T>
void arr_pop(Array<T>* array) {
	fm_assert(array->size && "cannot pop / what has not / been pushed");
	array->size -= 1;
	return;
}


// "Preallocate" space in the array without the cost of zeroing memory or default constructing
// entities. For example, if you want to push 16 elements to an array and modify the elements
// in-place -- reserve the space with this function, then just use the memory blocks.
template<typename T>
T* arr_reserve(Array<T>* array, int32 count) {
	fm_assert(array->size <= array->capacity + count && "Insufficient space remaining!");
	T* out = array->data + array->size;
	array->size += count;
	return out;
}


template<typename T>
T* arr_concat(Array<T>* dest, Array<T>* source) {
	fm_assert(dest->size + source->size < dest->capacity);
	memcpy(dest->data + dest->size, source->data, sizeof(T) * source->count);
	T* out = dest->data + dest->size;
	dest->size += source->size;
	return out;
}

template<typename T>
T* arr_concat(Array<T>* dest, Array<T>* source, int32 count) {
	fm_assert(dest->size + count < dest->capacity);
	memcpy(dest->data + dest->size, source->data, sizeof(T) * count);
	T* out = dest->data + dest->size;
	dest->size += count;
	return out;
}


template<typename T>
T* arr_back(Array<T>* array) {
	return array->data + (array->size - 1);
}

template<typename T>
T* arr_next(Array<T>* array) {
	fm_assert(array->size != array->capacity);
	return array->data + (array->size);
}

template<typename T>
void arr_free(Array<T>* array) {
	if (array->data) free(array->data);
	
	array->data = nullptr;
	array->size = 0;
	array->capacity = 0;
	return;
}

template<typename T>
int32 arr_bytes(Array<T>* array) {
	return array->capacity * sizeof(T);
}

template<typename T>
int32 arr_bytes_used(Array<T>* array) {
	return array->size * sizeof(T);
}

#define arr_for(array, it) for (auto (it) = (array).data; (it) != ((array).data + (array).size); (it)++)
#define arr_rfor(array, it) for (auto (it) = (array).data + array.size - 1; (it) >= ((array).data); (it)--)



template<typename T>
struct ArrayView {
	int32 size      = 0;
	int32 capacity  = 0;
	T* data         = nullptr;

	// @spader could make this const correct, whatever
	T* operator[](uint32 index) { fm_assert(index < size); return data + index; }
};

template<typename T>
ArrayView<T> arr_view(T* data, int32 size) {
	ArrayView<T> view;
	view.size = size;
	view.capacity = size;
	view.data = data;
	
	return view;
}

template<typename T>
ArrayView<T> arr_view(Array<T>* array) {
	ArrayView<T> view;
	view.size = array->size;
	view.capacity = array->size;
	view.data = array->data;
	
	return view;
}

template<typename T>
ArrayView<T> arr_view(Array<T>* array, int32 index, int32 count) {
	fm_assert(index >= 0);
	fm_assert(index + count <= array->capacity);

	ArrayView<T> view;
	view.size = count;
	view.capacity = count;
	view.data = array->data + index;
	
	return view;
}

template<typename T, int32 N>
ArrayView<T> arr_view(T (&array)[N]) {
	ArrayView<T> view;
	view.size = N;
	view.capacity = N;
	view.data = array;
	
	return view;
}


template<typename T>
int32 arr_indexof(ArrayView<T>* array, T* element) {
	int32 index = element - array->data;
	fm_assert(index >= 0);
	fm_assert(index < array->size);
	return index;
}


template<typename T>
struct ArrayMarker {
	int32 begin = 0;
	int32 frozen_size = -1;
	Array<T>* array  = nullptr;
};

template<typename T>
ArrayMarker<T> arr_marker_make(Array<T>* array) {
	ArrayMarker<T> marker;
	arr_marker_init(&marker, array);
	return marker;
}

template<typename T>
void arr_marker_init(ArrayMarker<T>* marker, Array<T>* array) {
	marker->begin = array->size;
	marker->array = array;
}

template<typename T>
void arr_marker_freeze(ArrayMarker<T>* marker) {
	assert(marker->array);
	marker->frozen_size = marker->array->size;
}

template<typename T>
int32 arr_marker_count(ArrayMarker<T>* marker) {
	if (marker->frozen_size >= 0) return marker->frozen_size - marker->begin;
	return marker->array->size - marker->begin;
}
