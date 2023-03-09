void init_imgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto& imgui = ImGui::GetIO();
	imgui.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//imgui.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	imgui.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::StyleColorsDark();

	imgui.IniFilename = nullptr;

	auto& style = ImGui::GetStyle();
	style.FrameRounding = 6;
	style.WindowRounding = 6;
	style.WindowBorderSize = 0;
	
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.17f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.15f, 0.23f, 0.34f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.15f, 0.32f, 0.54f, 1.00f);
	colors[ImGuiCol_Separator]              = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.09f, 0.13f, 1.00f);

	ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void update_imgui() {
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void render_imgui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
