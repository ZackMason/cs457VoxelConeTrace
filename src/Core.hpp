#pragma once
#pragma once

#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <cmath>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

constexpr int BIT(int x)
{
	return 1 << x;
}

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using v2f = glm::vec2;
using v3f = glm::vec3;
using v4f = glm::vec4;

using m33 = glm::mat3;
using m44 = glm::mat4;
using m22 = glm::mat2;

#if 1
template <typename T>
inline T lerp(T a, T b, f32 t)
{
    return b * t + a * (1.0f-t);
}
#endif


inline f32 lerp(f32 a, f32 b, f32 r)
{
	return b * r + (1.0f - r) * a;
}

inline f32 lerp_dt(f32 a, f32 b, f32 s, f32 dt)
{
	return lerp(a, b, 1.0f - std::pow(s, dt));
}

inline f32 damp(f32 a, f32 b, f32 lambda, f32 dt)
{
	return lerp(a, b, 1.0f - std::exp(-lambda * dt));
}

