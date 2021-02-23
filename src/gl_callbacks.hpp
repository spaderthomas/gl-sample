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
