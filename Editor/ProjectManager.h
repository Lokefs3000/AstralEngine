#pragma once

#include <string>
#include <memory>
#include <functional>

struct AstralObject;

class ProjectManager {
private:
	bool m_NewProjectVisible = false;
	std::string m_ProjectName = "New project";

	std::shared_ptr<AstralObject> m_Projects;
public:
	ProjectManager();

	void Render(std::function<void(std::string)> func);
};