struct OpenGL {
	uint32 vao;
	uint32 buffer;
};
OpenGL opengl;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void init_render() {
	// Generate OpenGL resources
	glGenVertexArrays(1, &opengl.vao);
	glGenBuffers(1, &opengl.buffer);

	// Describe the layout of our vertex buffer
	glBindVertexArray(opengl.vao);
	glBindBuffer(GL_ARRAY_BUFFER, opengl.buffer);

	auto stride = sizeof(Vector3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, 0);
	glEnableVertexAttribArray(0);

	// Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void clear_render_target() {
	glClearColor(CLEAR_COLOR);
	glClear(GL_COLOR_BUFFER_BIT);
}

void swap_buffers() {
	glfwSwapBuffers(window.handle);
}

void update_render() {
	clear_render_target();

	glBindVertexArray(opengl.vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, opengl.buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	auto shader = find_shader("solid");
	shader->begin();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	shader->end();
	
	render_imgui();
	swap_buffers();
}
