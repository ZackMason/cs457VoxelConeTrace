#pragma once

#include "Core.hpp"

struct Node;

struct IntersectData
{
	bool intersect = false;
	f32 distance = 0.0f;
	Node* node = nullptr;
	f32 time = 0.0;
	v3f position = v3f(0, 0, 0);
    
	operator bool() { return intersect; }
};

//std::ostream& operator<<(std::ostream& os, const IntersectData& data);
