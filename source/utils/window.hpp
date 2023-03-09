struct window_flags {
	bool windowed = true;
	bool border = true;
};

enum class WindowFlags {
	None = 0,
	Windowed = 1 << 0,
	Border = 1 << 1,
	Vsync = 1 << 2
};
DEFINE_ENUM_FLAG_OPERATORS(WindowFlags)

enum class DisplayMode {
	p480,
	p1080,
	p1440,
	p2160,
};

struct WindowInfo {
	Vector2I native_resolution;
	Vector2I output_resolution;
	DisplayMode display_mode;
	WindowFlags flags;
	GLFWwindow* handle;
};
WindowInfo window;

void init_glfw();
void set_native_resolution(float32 width, float32 height);
void set_display_mode(DisplayMode mode);
void set_window_flags(WindowFlags flags);
float32 get_display_scale();
