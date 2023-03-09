#define INPUT_MASK_EDITOR 0x1
#define INPUT_MASK_GAME   0x2
#define INPUT_MASK_ALL    0xFF

#define GLFW_KEY_CONTROL 349
#define GLFW_KEY_SUPER 350
#define GLFW_KEY_SHIFT 351
#define GLFW_KEY_ALT 352

struct InputManager {
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
	Vector2 camera;
	Vector2 mouse;
	Vector2 scroll;

	uint8 mask;
	uint8 old_mask;

	bool is_down[GLFW_KEY_LAST];
	bool was_down[GLFW_KEY_LAST];
	char shift_map[128];

	InputManager();

	void enable_channel(int8 channel);
	void disable_channel(int8 channel);
	void start_imgui();
	void stop_imgui();
	bool was_pressed(int32 id, int channel = INPUT_MASK_ALL);
	bool was_released(int32 id, int channel = INPUT_MASK_ALL);
	bool is_mod_down(int32 mod_key, int channel = INPUT_MASK_ALL);
	bool chord(int32 mod_key, int32 cmd_key, int channel = INPUT_MASK_ALL);
	void begin_frame();
	void end_frame();	
	void fill_shift_map();
	void check_focus();
};

InputManager& get_input_manager();
void update_input();

// GLFW Callbacks
void GLFW_Cursor_Pos_Callback(GLFWwindow* window, double xpos, double ypos);
void GLFW_Mouse_Button_Callback(GLFWwindow* window, int button, int action, int mods);
void GLFW_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFW_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void GLFW_Error_Callback(int err, const char* msg);
void GLFW_Window_Size_Callback(GLFWwindow* window, int width, int height);
