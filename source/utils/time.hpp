struct TimeMetric {
	RingBuffer<float64> queue;
	float64 time_begin;

	void init();
	void begin();
	void end();
	void busy_wait(float64 target);
	float64 get_average();
	float64 get_last();
};

struct TimeMetrics {
	TimeMetric frame;
	TimeMetric update;
	TimeMetric render;

	float32 target_fps;
	float32 dt;
};
TimeMetrics time_metrics;

void init_time();
void update_time();
void set_target_fps(float32 fps);
