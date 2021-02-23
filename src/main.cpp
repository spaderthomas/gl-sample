#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Globals
GLFWwindow* g_window;

GLuint vao;
GLuint vertex_buffer;
GLuint element_buffer;

std::vector<float> vertices = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};
std::vector<uint> indices = {
	0, 1, 2,
};

// Utilities
void tdlog(std::string message) {
	std::cout << message << std::endl;
}

#define tdns_find(vector, item) (find((vector).begin(), (vector).end(), (item)) != (vector).end()) 
#define fox_for(iterName, iterCount) for (unsigned int iterName = 0; iterName < (iterCount); ++iterName)

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
	std::string paths[] = {
		vs_path,
		fs_path
	};

	unsigned int shader_program;
	shader_program = glCreateProgram();

	fox_for(ishader, 2) {
		// Read in shader data
		std::string path = paths[ishader];
		FILE *shader_source_file = fopen(path.c_str(), "rb");
		fseek(shader_source_file, 0, SEEK_END);
		unsigned int fsize = ftell(shader_source_file);
		fseek(shader_source_file, 0, SEEK_SET);

		// @leak
		char *source = (char*)malloc(fsize + 1);
		fread(source, fsize, 1, shader_source_file);
		source[fsize] = 0;
		fclose(shader_source_file);

		// Compile the shader
		unsigned int shader_kind = (ishader == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		unsigned int shader = glCreateShader(shader_kind);

		glShaderSource(shader, 1, &source, NULL);

		glCompileShader(shader);
		if (glGetError()) {
			tdlog("Error compiling shader: " + name);
		}
		glAttachShader(shader_program, shader);
	}
		
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
	std::string vs = "/home/spader/programming/gl-starter/shaders/solid.vs";
	std::string fs = "/home/spader/programming/gl-starter/shaders/solid.fs";
	std::string name = "solid";
	solid_shader.init(vs, fs, name);
}

// transform
struct SRT {
	glm::vec2 translate;
	glm::vec2 scale;
	float rad_rot;
	
	static SRT no_transform();
};

glm::mat3 identity_mat3();
glm::mat3 mat3_from_transform(SRT transform);

SRT SRT::no_transform() {
	SRT t;
	t.scale = glm::vec2(1.0f);
	t.rad_rot = 0.f;
	t.translate = glm::vec3(0.0f);
	return t;
}

glm::mat3 identity_mat3() {
	return glm::mat3(1.f);
}

// glm is column then row (array of columns_
glm::mat3 mat3_from_transform(SRT transform) {
	glm::mat3 scale_mat = identity_mat3();
	scale_mat[0][0] = transform.scale.x;
	scale_mat[1][1] = transform.scale.y;
	scale_mat[2][2] = 1.f;

	glm::mat3 rot_mat = identity_mat3();
	rot_mat[0][0] = glm::cos(transform.rad_rot);
	rot_mat[0][1] = glm::sin(transform.rad_rot);
	rot_mat[1][0] = -1.f * glm::sin(transform.rad_rot);
	rot_mat[1][1] = glm::cos(transform.rad_rot);

	glm::mat3 trans_mat = identity_mat3();
	trans_mat[2][0] = transform.translate.x;
	trans_mat[2][1] = transform.translate.y;
	trans_mat[2][2] = 1.f;

	return trans_mat * rot_mat * scale_mat;
}

// GLFW Callbacks
void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void *userParam) {
	(void)userParam;
	
	switch (id) {
		case 131169: // The driver allocated storage for renderbuffer
		return;
		case 131185: // glBufferData
		return;
		case 481131: // buffer info
		return;
		case 131184: // buffer info
		return;
		case 131204: // Weird base level inconsistent bug. It may be a driver bug.
		return;
	}
	
	std::string debug_msg;
	debug_msg += "OpenGL Debug Message: ";
	debug_msg += "\nSource: ";
	switch (source) {
		case GL_DEBUG_SOURCE_API:
		debug_msg += "API";
		break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		debug_msg += "Window System";
		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
		debug_msg += "Shader Compiler";
		break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
		debug_msg += "Third Party";
		break;
		case GL_DEBUG_SOURCE_APPLICATION:
		debug_msg += "Application";
		break;
		case GL_DEBUG_SOURCE_OTHER:
		debug_msg += "Other";
		break;
	}
	
	debug_msg += "\nType: ";
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:
		debug_msg += "Error";
		break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debug_msg += "Deprecated Behaviour";
		break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debug_msg += "Undefined Behaviour";
		break;
		case GL_DEBUG_TYPE_PORTABILITY:
		debug_msg += "Portability";
		break;
		case GL_DEBUG_TYPE_PERFORMANCE:
		debug_msg += "Performance";
		break;
		case GL_DEBUG_TYPE_MARKER:
		debug_msg += "Marker";
		break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
		debug_msg += "Push Group";
		break;
		case GL_DEBUG_TYPE_POP_GROUP:
		debug_msg += "Pop Group";
		break;
		case GL_DEBUG_TYPE_OTHER:
		debug_msg += "Other";
		break;
	}
	
	debug_msg += "\nID: ";
	debug_msg += std::to_string(id);
	
	debug_msg += "\nSeverity: ";
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
		debug_msg += "High";
		break;
		case GL_DEBUG_SEVERITY_MEDIUM:
		debug_msg += "Medium";
		break;
		case GL_DEBUG_SEVERITY_LOW:
		debug_msg += "Low";
		break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		debug_msg += "Notification";
		break;
	}
	
	debug_msg += "\nGL message: " + std::string(message);
	debug_msg += "\n\n";
	tdlog(debug_msg);
}

static void GLFW_Cursor_Pos_Callback(GLFWwindow* window, double xpos, double ypos) {
	std::cout << "cursor: " << std::to_string(xpos) << ", " << std::to_string(ypos) << std::endl;
}

void GLFW_Mouse_Button_Callback(GLFWwindow* window, int button, int action, int mods) {
	std::cout << "mouse: " << std::to_string(button) << ", " << std::to_string(action) << std::endl;
	}

void GLFW_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << "key: " << std::to_string(key) << ", " << std::to_string(action) << std::endl;
}

void GLFW_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset) {
	std::cout << "scroll: " << std::to_string(xoffset) << ", " << std::to_string(yoffset) << std::endl;
}

void GLFW_Error_Callback(int err, const char* msg) {
	std::cout << err;
	std::cout << msg;
}

void GLFW_Window_Size_Callback(GLFWwindow* window, int width, int height) {
	std::cout << "window size: " << std::to_string(width) << ", " << std::to_string(height) << std::endl;
}

int init_glfw() {
	auto result = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	glfwSetErrorCallback(GLFW_Error_Callback);

	g_window = glfwCreateWindow(1280, 720, "mr newmans adventure", NULL, NULL);
	if (g_window == NULL) {
		tdlog("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		tdlog("Failed to initialize GLAD");
		return -1;
	}
	
	glfwSetCursorPosCallback(g_window, GLFW_Cursor_Pos_Callback);
	glfwSetMouseButtonCallback(g_window, GLFW_Mouse_Button_Callback);
	glfwSetKeyCallback(g_window, GLFW_Key_Callback);
	glfwSetScrollCallback(g_window, GLFW_Scroll_Callback);
	glfwSetWindowSizeCallback(g_window, GLFW_Window_Size_Callback);
	
	glfwSwapInterval(0);

	return 0;
}

void init_gl() {
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallbackKHR(gl_debug_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &element_buffer);
	
	
	// Fill GPU sprite buffers
	glBindVertexArray(vao);
	
	// Send all the data to OpenGL buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

int main(int arg_count, char** args) {
	std::cout << "Hello, Evan!" << std::endl;

	init_glfw();
	init_shaders();
	init_gl();
	
	while(!glfwWindowShouldClose(g_window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);


		solid_shader.begin();
		
		auto transform = SRT::no_transform();
		transform.scale = { 1.f, 1.f };
		auto trans_mat = mat3_from_transform(transform);
		solid_shader.set_mat3("transform", trans_mat);


		glm::vec4 color = { 1.f, 0.f, 0.f, 1.f };
		solid_shader.set_vec4("color", color);

		solid_shader.check();
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		solid_shader.end();

		glfwSwapBuffers(g_window);
	}
}
