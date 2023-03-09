size_t hash_string(const char* string) {
	constexpr size_t prime = 31;
	
	size_t result = 0;
	size_t len = strlen(string);
	for (int i = 0; i < len; i++) {
        result = string[i] + (result * prime);
    }
    return result;
}
