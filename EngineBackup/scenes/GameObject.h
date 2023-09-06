#pragma once

#include "Variables.h"

#include "components/EntityComponents.h"

#include <vector>
#include <memory>
#include <iostream>

class EXPORT GameObject {
public:
	Entity Id = ULLONG_MAX;

	std::shared_ptr<GameObject> Parent = NULL;
	std::vector<std::shared_ptr<GameObject>> Children;

	std::shared_ptr<EntityComponents> Components;

	void SetParent(std::shared_ptr<GameObject> parent, std::shared_ptr<GameObject> self) {
		if (Parent.get() != NULL) {
			Parent->RemoveChild(self);
		}
		Parent = parent;
		if (Parent.get() != NULL) {
			Parent->AddChild(self);
		}
	}

	void AddChild(std::shared_ptr<GameObject> child) {
		Children.push_back(child);
	}

	void RemoveChild(std::shared_ptr<GameObject> child) {
		auto find = std::find(Children.begin(), Children.end(), child);
		if (find != Children.end()) {
			Children.erase(find);
		}
	}
};