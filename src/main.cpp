#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include "gl_callbacks.hpp"


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
	// Give GL a debug callback that does not buffer messages if we're in debug mode
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

	// Generate GL objects
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &element_buffer);

	// Bind the VAO so all subsequent GL calls affect it
	glBindVertexArray(vao);

	// Fill the vertex buffer with data + describe data layout
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Fill the element buffer with data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
	

}

int main(int arg_count, char** args) {
	std::cout << "Hello, Evan!" << std::endl;

	init_glfw();
	init_shaders();
	init_gl();
	
	while(!glfwWindowShouldClose(g_window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind the GL objects that we filled with data
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		// Set our shader inputs
		solid_shader.begin();
		
		auto transform = SRT::no_transform();
		transform.scale = { 1.f, 1.f };
		solid_shader.set_mat3("transform", mat3_from_transform(transform));

		glm::vec4 color = { 1.f, 0.f, 0.f, 1.f };
		solid_shader.set_vec4("color", color);

		// This ensures that all the shader's inputs have been set
		solid_shader.check();

		// Do a draw :3
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

		// Reset the shader for the next frame
		solid_shader.end();

		glfwSwapBuffers(g_window);
	}
}
