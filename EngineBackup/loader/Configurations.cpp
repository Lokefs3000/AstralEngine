#include "pch.h"
#include "Configurations.h"

std::pair<std::string, std::string> Configurations::GetLineVariable(std::string line)
{
    std::string name = "";
    std::string value = "";

    uint32_t findId = line.find_first_of(":");
    if (findId == std::string::npos) {
        return std::pair<std::string, std::string>("NO", "RESULT");
    }

    name = line.substr(0, findId);
    value = line.substr(findId+1);

    std::pair<std::string, std::string> lineVar = std::make_pair(StringUtils::Trim(name), StringUtils::Trim(value));

    return lineVar;
}

void Configurations::HandleLineData(std::pair<std::string, std::string> data, std::shared_ptr<ConfigObject> current)
{
    try
    {
        int value = std::stoi(data.second);
        current->Values.insert(std::make_pair(data.first, value));
        return;
    }
    catch (const std::exception& ex) {}

    try
    {
        float value = std::stof(data.second);
        current->Values.insert(std::make_pair(data.first, value));
        return;
    }
    catch (const std::exception& ex) {}

    if (data.second == "true") {
        current->Values.insert(std::make_pair(data.first, true));
        return;
    }
    if (data.second == "false") {
        current->Values.insert(std::make_pair(data.first, false));
        return;
    }

    current->Values.insert(std::make_pair(data.first, data.second));
}

void Configurations::HandleObject(uint32_t& line, std::vector<std::string> lines, std::shared_ptr<ConfigObject> current, std::shared_ptr<ConfigObject> root)
{
    line++;
    for (uint32_t i = 0; line < lines.size(); line++)
    {
        if (lines[line] == "}") {
            root->Children.push_back(current);
            break;
        }
        else {
            HandleLine(line, lines, current);
        }
    }
}

void Configurations::HandleLine(uint32_t& line, std::vector<std::string> lines, std::shared_ptr<ConfigObject> root)
{
    for (uint32_t i = 0; line < lines.size(); line++)
    {
        std::pair<std::string, std::string> value = GetLineVariable(lines[line]);

        if (value.second == "{") {
            if (value.first.empty() || value.first == "ROOT")
                continue; //TODO: Throw error about name being defined and not being called ROOT

            std::shared_ptr<ConfigObject> current = std::make_shared<ConfigObject>();
            current->Name = value.first;

            if (root->Name == "ROOT")
                root->Children.push_back(current);

            HandleObject(line, lines, current, root);
        }
        else {
            if (root->Name == "ROOT")
                continue; //TODO: Throw error about not defining variables in root
            HandleLineData(value, root);
            break;
        }
    }
}

std::shared_ptr<ConfigObject> Configurations::LoadConfig(std::string data)
{
    Configurations* config = new Configurations();
    std::shared_ptr<ConfigObject> co = config->LoadConfigLocal(data);
    delete config;
    return co;
}

std::shared_ptr<ConfigObject> Configurations::LoadConfigLocal(std::string data)
{
    std::istringstream buffer(data);
    std::string line;

    std::shared_ptr<ConfigObject> Root = std::make_shared<ConfigObject>();
    Root->Name = "ROOT";

    std::vector<std::string> lines;
    while (std::getline(buffer, line))
        if (!line.empty())
            lines.push_back(StringUtils::Trim(line));

    for (uint32_t i = 0; i < lines.size(); i++)
        HandleLine(i, lines, Root);

    return Root;
}
