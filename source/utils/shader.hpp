#define MAX_UNIFORMS 16
#define MAX_UNIFORM_LEN 32

struct Shader {
	char name    [MAX_PATH_LEN] = {0};
	char vs_path [MAX_PATH_LEN] = {0};
	char fs_path [MAX_PATH_LEN] = {0};
	uint id = 0;
	uint num_uniforms = 0;
	Array<char[MAX_UNIFORM_LEN]> uniforms_set_this_call;
	
	static int active;

	void init(const char* vs_path, const char* fs_path, const char* name);
	unsigned int get_uniform_loc(const char* name);
	bool was_uniform_set(const char* name);
	void mark_uniform_set(const char* name);

	void set_int(const char* name,   int32   data);
	void set_float(const char* name, float32 data);
	void set_vec2(const char* name,  Vector2 data);
	void set_vec3(const char* name,  Vector3 data);
	void set_vec4(const char* name,  Vector4 data);
	void set_mat4(const char* name,  Matrix4 data);

	void begin();
	void check();
	void end();
};
int Shader::active = -1;

Array<Shader> shaders;

void init_shaders();
Shader* find_shader(const char* name);
void add_shader(const char* name);

struct ShaderManager {};
ShaderManager& get_shader_manager();	
