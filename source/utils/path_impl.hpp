void normalize_path(char* str) {
	int i = 0;
	while (true) {
		if (str[i] == 0) break;
		if (str[i] == '\\') {
			str[i] = '/';
		}
		i++;
	}
}

char* extract_file_name(char* full_path) {
	auto size = (int32)strlen(full_path);
	auto index = size - 1;
	while (true) {
		if (index < 0) break;
		if (full_path[index] == '/') { index += 1; break; }
		index -= 1;
	}
	
	auto length = size - index; 
	char* file_name = (char*)temporary_storage.alloc(length + 1);
	strncpy(file_name, full_path + index, length);
	return file_name;
} 

bool is_directory(char* path) {
	auto attribute = GetFileAttributesA(path);
	if (attribute == INVALID_FILE_ATTRIBUTES) return false;
	return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

char* wide_to_utf8(uint16* string, uint32 length) {
	char* utf8 = (char*)temporary_storage.alloc(length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)string, length, utf8, length, NULL, NULL);
	return utf8;
}

void find_install_dir(char* buf, int32 n) {
	GetModuleFileNameA(NULL, buf, n);
	auto len = (int32)strlen(buf);

	// Normalize
	for (int32 i = 0; i < len; i++) {
		if (buf[i] == '\\') buf[i] = '/';
	}

	// Remove executable name AND build directory -- this is just how Visual Studio sets up
	// build folders for solutions. This will be different in a release build where your
	// executable might be e.g. right next to your assets
	int32 removed = 0;
	for (int32 i = len - 1; i > 0; i--) {
		if (buf[i] == '/') removed++;
		buf[i] = 0;
		if (removed == 5) break;
	}
}

void init_paths() {
	find_install_dir(gfx_path::install_dir, gfx_path::max);
}

namespace gfx_path {
	tstring vertex_shader(const char* name) {
		auto path = temporary_storage.alloc_path();
		snprintf(path, gfx_path::max, "%s/asset/%s.vs", install_dir, name);
		return path;
	}

	tstring fragment_shader(const char* name) {
		auto path = temporary_storage.alloc_path();
		snprintf(path, gfx_path::max, "%s/asset/%s.fs", install_dir, name);
		return path;
	}
}
