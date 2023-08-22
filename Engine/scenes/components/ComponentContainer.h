#pragma once

#include "Transform.h"

struct ComponentContainer
{
	Transform transform;

	template<typename T>
	T& operator[](int idx) {
		switch (idx)
		{
		case 1:
			return transform;
		default:
			break;
		}

		T t = T();
		return t;
	}
};