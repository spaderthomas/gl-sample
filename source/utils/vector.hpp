struct Vector2;
struct Vector2I;
struct Vector3;
struct Vector4;

struct Vector2 {
	float32 x = 0;
	float32 y = 0;

	Vector2& operator=(const Vector2I& other);
	Vector2 operator*(const float scalar);
	Vector2& operator*=(const float scalar);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
};

struct Vector2I {
	int32 x = 0;
	int32 y = 0;
};

struct Vector3 {
	float32 x = 0;
	float32 y = 0;
	float32 z = 0;
};

struct Vector4 {
	union {
		float32 x = 0;
		float32 r;
		float32 bottom;
	};
	union {
		float32 y = 0;
		float32 g;
		float32 top;
	};
	union {
		float32 z = 0;
		float32 b;
		float32 left;
	};
	union {
		float32 w = 0;
		float32 a;
		float32 right;
	};

	float32& operator[](int32 index) {
		fm_assert(index >= 0);
		fm_assert(index < 4);
		float32* data = (float32*)this;
		return *(data + index);
	}
};

Vector2 vec_sub(Vector2* a, Vector2* b) {
	return { a->x - b->x, a->y - b->y };
}

float32 vec_dot(Vector2* a, Vector2* b) {
	return (a->x * b->x + a->y * b->y);
}

Vector2& Vector2::operator=(const Vector2I& other) {
	x = (float32)other.x;
	y = (float32)other.y;
	return *this;
}

Vector2 Vector2::operator*(const float scalar) {
	Vector2 other = *this;
	other.x *= scalar;
	other.y *= scalar;
	return other;
}

Vector2& Vector2::operator*=(const float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}
