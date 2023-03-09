InputManager::InputManager() {
	fill_shift_map();
}

void InputManager::enable_channel(int8 channel) {
	mask |= channel;
}
	
void InputManager::disable_channel(int8 channel) {
	mask &= ~channel;
}
	
bool InputManager::was_pressed(int32 id, int channel) {
	if (!(this->mask & channel)) return false;
	return is_down[id] && !was_down[id];
}
	
bool InputManager::was_released(int32 id, int channel) {
	if (!(this->mask & channel)) return false;
	return !is_down[id] && was_down[id];
}
	
bool InputManager::is_mod_down(int32 mod_key, int channel) {
	if (!(this->mask & channel)) return false;
		
	bool down = false;
	if (mod_key == GLFW_KEY_CONTROL) {
		down |= is_down[GLFW_KEY_RIGHT_CONTROL];
		down |= is_down[GLFW_KEY_LEFT_CONTROL];
	}
	if (mod_key == GLFW_KEY_SUPER) {
		down |= is_down[GLFW_KEY_LEFT_SUPER];
		down |= is_down[GLFW_KEY_RIGHT_SUPER];
	}
	if (mod_key == GLFW_KEY_SHIFT) {
		down |= is_down[GLFW_KEY_LEFT_SHIFT];
		down |= is_down[GLFW_KEY_RIGHT_SHIFT];
	}
	if (mod_key == GLFW_KEY_ALT) {
		down |= is_down[GLFW_KEY_LEFT_ALT];
		down |= is_down[GLFW_KEY_RIGHT_ALT];
	}

	return down;
}

bool InputManager::chord(int32 mod_key, int32 cmd_key, int channel) {
	if (!(this->mask & channel)) return false;
	return is_mod_down(mod_key, channel) && was_pressed(cmd_key, channel);
}
	
void InputManager::fill_shift_map() {
	memset(shift_map, 0, sizeof(shift_map));
		
	shift_map[' ']  =  ' ';
	shift_map['\''] =  '"';
	shift_map[',']  =  '<';
	shift_map['-']  =  '_';
	shift_map['.']  =  '>';
	shift_map['/']  =  '?';

	shift_map['0']  =  ')';
	shift_map['1']  =  '!';
	shift_map['2']  =  '@';
	shift_map['3']  =  '#';
	shift_map['4']  =  '$';
	shift_map['5']  =  '%';
	shift_map['6']  =  '^';
	shift_map['7']  =  '&';
	shift_map['8']  =  '*';
	shift_map['9']  =  '(';

	shift_map[';']  =  ':';
	shift_map['=']  =  '+';
	shift_map['[']  =  '{';
	shift_map['\\'] =  '|';
	shift_map[']']  =  '}';
	shift_map['`']  =  '~';
		
	shift_map['a']  =  'A';
	shift_map['b']  =  'B';
	shift_map['c']  =  'C';
	shift_map['d']  =  'D';
	shift_map['e']  =  'E';
	shift_map['f']  =  'F';
	shift_map['g']  =  'G';
	shift_map['h']  =  'H';
	shift_map['i']  =  'I';
	shift_map['j']  =  'J';
	shift_map['k']  =  'K';
	shift_map['l']  =  'L';
	shift_map['m']  =  'M';
	shift_map['n']  =  'N';
	shift_map['o']  =  'O';
	shift_map['p']  =  'P';
	shift_map['q']  =  'Q';
	shift_map['r']  =  'R';
	shift_map['s']  =  'S';
	shift_map['t']  =  'T';
	shift_map['u']  =  'U';
	shift_map['v']  =  'V';
	shift_map['w']  =  'W';
	shift_map['x']  =  'X';
	shift_map['y']  =  'Y';
	shift_map['z']  =  'Z';
}

InputManager& get_input_manager() {
	static InputManager manager;
	return manager;
}

void update_input() {
	auto& input = get_input_manager();

	for (int key = 0; key < GLFW_KEY_LAST; key++) {
		input.was_down[key] = input.is_down[key];
	}

	input.scroll.x = 0;
	input.scroll.y = 0;

	// Fill the input manager's buffers with what GLFW tells us
	glfwPollEvents();

	// Determine whether ImGui is requesting key inputs
	ImGuiIO& imgui = ImGui::GetIO();

	// If ImGui isn't actively asking for input, don't give it anything
	bool give_input_to_imgui = false;
	give_input_to_imgui |= imgui.WantCaptureKeyboard;
	give_input_to_imgui |= imgui.WantCaptureMouse;
	if (give_input_to_imgui) {
		input.disable_channel(INPUT_MASK_GAME);
		input.enable_channel(INPUT_MASK_EDITOR);
	} else {
		input.disable_channel(INPUT_MASK_EDITOR);
		input.enable_channel(INPUT_MASK_GAME);
	}
}

////////////////////
// GLFW Callbacks //
////////////////////
static void GLFW_Cursor_Pos_Callback(GLFWwindow* glfw, double x, double y) {
	auto& input_manager = get_input_manager();
	
	auto fx = (float32)x;
	if (fx < 0) fx = 0;
	
	auto fy = (float32)y;
	if (fy < 0) fy = 0;

	input_manager.mouse.x = fx;
	input_manager.mouse.y = fy;
}

void GLFW_Mouse_Button_Callback(GLFWwindow* window, int button, int action, int mods) {
	auto& input_manager = get_input_manager();
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_LEFT] = true;
		}
		if (action == GLFW_RELEASE) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_RIGHT] = true;
		}
		if (action == GLFW_RELEASE) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_RIGHT] = false;
		}
	}
}

void GLFW_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto& manager = get_input_manager();
	
	if (action == GLFW_PRESS) {
		manager.is_down[key] = true;
	}
    if (action == GLFW_RELEASE) {
		manager.is_down[key] = false;
	}
}

void GLFW_Scroll_Callback(GLFWwindow* window, double dx, double dy) {
	auto& input_manager = get_input_manager();

	input_manager.scroll.x = (float32)dx;
	input_manager.scroll.y = (float32)dy;
}

void GLFW_Error_Callback(int err, const char* msg) {
}

void GLFW_Window_Size_Callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);	
}

