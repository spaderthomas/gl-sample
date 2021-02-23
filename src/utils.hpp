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
