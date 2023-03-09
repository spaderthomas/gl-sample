void init_time() {
	set_target_fps(60);
	
	time_metrics.frame.init();
	time_metrics.update.init();
	time_metrics.render.init();
}

void update_time() {
	time_metrics.frame.end();
	time_metrics.frame.busy_wait(time_metrics.dt);
}

void TimeMetric::init() {
	rb_init(&this->queue, 64);
}

void TimeMetric::begin() {
	this->time_begin = glfwGetTime();
}

void TimeMetric::end() {
	auto time_end = glfwGetTime();
	auto delta = time_end - this->time_begin;
	rb_push_overwrite(&this->queue, delta);
}

float64 TimeMetric::get_average() {
	if (!queue.size) return 0;
	
	float64 total = 0;
	rb_for(queue, entry) {
		total += **entry;
	}

	return total / queue.size;
}

float64 TimeMetric::get_last() {
	if (queue.size) return *rb_back(&queue);
	return 0;
}

void TimeMetric::busy_wait(float64 target) {
	while (true) {
		auto delta = glfwGetTime() - this->time_begin;
		if (delta >= target) break;
	}
}

void set_target_fps(float32 fps) {
	time_metrics.target_fps = fps;
	time_metrics.dt = 1.f / fps;
}
