// Shader
struct Shader {
	std::string name;
	uint id;
	uint num_uniforms;
	std::vector<std::string> uniforms_set_this_call;
	
	static int active;

	void init(std::string vs_path, std::string fs_path, std::string name);
	unsigned int get_uniform_loc(const char* name);

	void set_vec4(const char* name, glm::vec4 vec);
	void set_vec3(const char* name, glm::vec3 vec);
	void set_vec2(const char* name, glm::vec2 vec);
	void set_mat3(const char* name, glm::mat3 mat);
	void set_mat4(const char* name, glm::mat4 mat);
	void set_int(const char* name, GLint val);
	void set_float(const char* name, GLfloat val);

	void begin();
	void check();
	void end();
};
int Shader::active = -1;

Shader solid_shader;
void init_shaders();


void Shader::init(std::string vs_path, std::string fs_path, std::string name) {
	unsigned int shader_program;
	shader_program = glCreateProgram();

	auto load_shader = [&](auto path, auto kind) {
		// Read in shader data
		FILE *shader_source_file = fopen(path.c_str(), "rb");
		if (!shader_source_file) {
			tdlog("@invalid_shader_file: " + path);
		}
		fseek(shader_source_file, 0, SEEK_END);
		unsigned int fsize = ftell(shader_source_file);
		fseek(shader_source_file, 0, SEEK_SET);

		char *source = (char*)malloc(fsize + 1);
		fread(source, fsize, 1, shader_source_file);
		source[fsize] = 0;
		fclose(shader_source_file);

		// Compile the shader
		unsigned int shader = glCreateShader(kind);

		glShaderSource(shader, 1, &source, NULL);

		glCompileShader(shader);
		if (glGetError()) {
			tdlog("Error compiling shader: " + name);
		}
		glAttachShader(shader_program, shader);

		free(source);
	};
	load_shader(vs_path, GL_VERTEX_SHADER);
	load_shader(fs_path, GL_FRAGMENT_SHADER);

	// Link into a shader program
	glLinkProgram(shader_program);
	if (glGetError()) {
		tdlog("Error linking shader: " + std::string(vs_path));
	}
	id = shader_program;
	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, (int*)&num_uniforms);
	this->name = name;
}
unsigned int Shader::get_uniform_loc(const char* name) {
	auto loc = glGetUniformLocation(id, name);
	if (loc == -1) {
		tdlog("Tried to get uniform location, but it didn't exist. Uniform name: " + std::string(name) + ", Shader name: " + std::string(this->name));
		exit(1);
	}
	return loc;
}

void Shader::set_vec4(const char* name, glm::vec4 vec) {
	glUniform4f(get_uniform_loc(name), vec.x, vec.y, vec.z, vec.w);
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_vec3(const char* name, glm::vec3 vec) {
	glUniform3f(get_uniform_loc(name), vec.x, vec.y, vec.z);
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_vec2(const char* name, glm::vec2 vec) {
	glUniform2f(get_uniform_loc(name), vec.x, vec.y);
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_mat3(const char* name, glm::mat3 mat) {
	glUniformMatrix3fv(get_uniform_loc(name), 1, GL_FALSE, glm::value_ptr(mat));
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_mat4(const char* name, glm::mat4 mat) {
	glUniformMatrix4fv(get_uniform_loc(name), 1, GL_FALSE, glm::value_ptr(mat));
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_int(const char* name, GLint val) {
	glUniform1i(get_uniform_loc(name), val);
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}
void Shader::set_float(const char* name, GLfloat val) {
	glUniform1f(get_uniform_loc(name), val);
	if (!tdns_find(uniforms_set_this_call, name)) {
		uniforms_set_this_call.push_back(name);
	}
}

void Shader::begin() {
	if (Shader::active != -1) {
		tdlog("Called begin() with shader: " + name + ", but there was already an active shader. Did you forget to call end()?");
		exit(1);
	}
	glUseProgram(id);
	Shader::active = id;
	uniforms_set_this_call.clear();
}

void Shader::check() {
	if (uniforms_set_this_call.size() != num_uniforms) {
		std::string msg = "You didn't fill in all of the uniforms! Shader was: " + std::string(name);
		tdlog(msg);
		exit(1);
	}
	if (Shader::active != (int)id) {
		std::string msg = "Checked shader before draw, but it was not set. Did you forget to call begin()? Shader: " + name;
		tdlog(msg);
		exit(1);
	}
}

void Shader::end() {
	Shader::active = -1;
}

void init_shaders() {
	std::string vs = "shaders/solid.vs";
	std::string fs = "shaders/solid.fs";
	std::string name = "solid";
	solid_shader.init(vs, fs, name);
}
