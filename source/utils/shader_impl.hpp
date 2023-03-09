void Shader::init(const char* vs_path, const char* fs_path, const char* name) {
	arr_init(&uniforms_set_this_call, MAX_UNIFORMS);
	
	const char* paths[] = {
		vs_path,
		fs_path
	};

	int success;
	auto compilation_status = (char*)temporary_storage.alloc(512);

	unsigned int shader_program;
	shader_program = glCreateProgram();

	for (uint32 i = 0; i < 2; i++) {
		// Read in shader data
		const char* path = paths[i];
		FILE *shader_source_file = fopen(path, "rb");
		if (!shader_source_file) {
		}

		fseek(shader_source_file, 0, SEEK_END);
		unsigned int fsize = ftell(shader_source_file);
		fseek(shader_source_file, 0, SEEK_SET);

		char* source = (char*)calloc(fsize + 1, sizeof(char));
		defer { free(source); };
		
		fread(source, fsize, 1, shader_source_file);
		source[fsize] = 0;
		fclose(shader_source_file);

		// Compile the shader
		unsigned int shader_kind = (i == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		unsigned int shader = glCreateShader(shader_kind);

		glShaderSource(shader, 1, &source, NULL);

		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, compilation_status);
		}
		glAttachShader(shader_program, shader);
	}
		
	// Link into a shader program
	glLinkProgram(shader_program);
	glGetShaderiv(shader_program, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_program, 512, NULL, compilation_status);
	}

	// Push the data into the shader. If anything fails, the shader won't get
	// the new GL handles
	id = shader_program;
	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, (int*)&num_uniforms);

	strncpy(this->name, name, MAX_PATH_LEN);
	strncpy(this->vs_path, vs_path, MAX_PATH_LEN);
	strncpy(this->fs_path, fs_path, MAX_PATH_LEN);
}

unsigned int Shader::get_uniform_loc(const char* uniform) {
	// Shader failed to load properly. Don't spam the log.
	if (num_uniforms == 0) return -1;
	
	auto loc = glGetUniformLocation(id, uniform);
	if (loc == -1) {

	}
	return loc;
}

void Shader::mark_uniform_set(const char* name) {
	// Check if this uniform was already set. Setting it twice is OK.
	arr_for(uniforms_set_this_call, uniform) {
		if (!strncmp(*uniform, name, MAX_UNIFORM_LEN)) return;
	}

	// If it's a new uniform, check that we haven't already set all the uniforms.
	fm_assert(uniforms_set_this_call.size < MAX_UNIFORMS);

	// Copy the string and add this uniform
	char (*arr) [MAX_UNIFORM_LEN] = arr_next(&uniforms_set_this_call);
	char* buffer = &(*arr)[0];
	strncpy(buffer, name, MAX_PATH_LEN);
	uniforms_set_this_call.size += 1;
}

void Shader::set_vec4(const char* name, Vector4 vec) {
	glUniform4f(get_uniform_loc(name), vec.x, vec.y, vec.z, vec.w);
	mark_uniform_set(name);
}

void Shader::set_vec3(const char* name, Vector3 vec) {
	glUniform3f(get_uniform_loc(name), vec.x, vec.y, vec.z);
	mark_uniform_set(name);
}

void Shader::set_vec2(const char* name, Vector2 vec) {
	glUniform2f(get_uniform_loc(name), vec.x, vec.y);
	mark_uniform_set(name);
}

void Shader::set_mat4(const char* name, Matrix4 matrix) {
	glUniformMatrix4fv(get_uniform_loc(name), 1, GL_FALSE, (const float32*)&matrix);
	mark_uniform_set(name);
}

void Shader::set_int(const char* name, GLint val) {
	glUniform1i(get_uniform_loc(name), val);
	mark_uniform_set(name);
}

void Shader::set_float(const char* name, GLfloat val) {
	glUniform1f(get_uniform_loc(name), val);
	mark_uniform_set(name);
}

void Shader::begin() {
	if (Shader::active != -1) {
		return;
	}
	
	glUseProgram(id);
	Shader::active = id;
	arr_clear(&uniforms_set_this_call);
}

void Shader::check() {
	if (uniforms_set_this_call.size != num_uniforms) {
		return;
	}
	if (Shader::active != (int)id) {
		return;
	}
}

void Shader::end() {
	arr_clear(&uniforms_set_this_call);
	Shader::active = -1;
	glUseProgram(0);
}

void init_shaders() {
	arr_init(&shaders, 32);

	add_shader("solid");
}

Shader* find_shader(const char* name) {
	arr_for(shaders, shader) {
		if (!strncmp(shader->name, name, MAX_PATH_LEN)) return shader;
	}

	return nullptr;
}

void add_shader(const char* name) {
	auto shader = arr_push(&shaders);
	
	auto vertex_shader = gfx_path::vertex_shader(name);
	auto fragment_shader = gfx_path::fragment_shader(name);
	shader->init(vertex_shader, fragment_shader, name);
}

ShaderManager& get_shader_manager() {
	static ShaderManager manager;
	return manager;
}
