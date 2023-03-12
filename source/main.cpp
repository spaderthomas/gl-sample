// Library includes
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

GLFWwindow* window;

int main(int arg_count, char** args) {
	if (!glfwInit()) {
		auto error_code = glfwGetError(NULL);
		if (error_code != GLFW_NO_ERROR) {
			const char* description;
			int code = glfwGetError(&description);

			if (description) {
				printf("%s\n", description);
			}
		}
	}


	window = glfwCreateWindow(800, 600, "GFX", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {}
}

