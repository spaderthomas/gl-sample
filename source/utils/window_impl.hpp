void init_glfw() {
	set_window_flags(WindowFlags::Windowed | WindowFlags::Border | WindowFlags::Vsync);
	set_native_resolution(1920, 1080);
	set_display_mode(DisplayMode::p1440);
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Set a best guess for our default output resolution based on the monitor
	if (mode->width == 3840) {
		set_display_mode(DisplayMode::p2160);
	}
	else if (mode->width == 2560) {
		set_display_mode(DisplayMode::p1440);
	}
	else if (mode->width == 1920) {
		set_display_mode(DisplayMode::p1080);
	}
	else if (mode->width > 3840){
		set_display_mode(DisplayMode::p2160);
	}
	else {
		set_display_mode(DisplayMode::p1080);
	}
	
	// Translate the window flags into GLFW stuff that'll set up the window correctly	
	if (enum_any(window.flags & WindowFlags::Windowed)) {
		monitor = nullptr;
	}
	
	if (!enum_any(window.flags & WindowFlags::Border)) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}

	if (enum_any(window.flags & WindowFlags::Vsync)) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}

	// Init the window, give it a GL context, and load OpenGL
	window.handle = glfwCreateWindow(window.output_resolution.x, window.output_resolution.y, "GFX", monitor, NULL);
	glfwMakeContextCurrent(window.handle);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetCursorPosCallback(window.handle, GLFW_Cursor_Pos_Callback);
	glfwSetMouseButtonCallback(window.handle, GLFW_Mouse_Button_Callback);
	glfwSetKeyCallback(window.handle, GLFW_Key_Callback);
	glfwSetScrollCallback(window.handle, GLFW_Scroll_Callback);
	glfwSetWindowSizeCallback(window.handle, GLFW_Window_Size_Callback);
}

void set_native_resolution(float32 width, float32 height) {
	window.native_resolution.x = (int32)width;
	window.native_resolution.y = (int32)height;
}

void set_display_mode(DisplayMode mode) {
	window.display_mode = mode;
	if (mode == DisplayMode::p480) {
		window.output_resolution = { 854, 480 };
	}
	else if (mode == DisplayMode::p1080) {
		window.output_resolution = { 1920, 1080 };
	}
	else if (mode == DisplayMode::p1440) {
		window.output_resolution = { 2560, 1440 };
	}
	else if (mode == DisplayMode::p2160) {
		window.output_resolution = { 3840, 2160 };
	}
}

void set_window_flags(WindowFlags flags) {
	window.flags = flags;
}

float32 get_display_scale() {
	return (float32)window.output_resolution.x / (float32)window.native_resolution.x;
}
