void normalize_path(char* str);
bool is_directory(char* path);
char* extract_file_name(char* string);
char* wide_to_utf8(uint16* path, uint32 length);

namespace gfx_path {
	constexpr int max = 256;
	char install_dir [gfx_path::max];
	
	tstring vertex_shader();
	tstring fragment_shader();
}

void find_install_dir(char* buf, int32 n);
void init_paths();
