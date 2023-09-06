#include "pch.h"
#include "AstralLoader.h"

#include "Data/AstralData.h"

#include "Utlities/StringUtils.h"

#include <sstream>

std::pair<std::string, std::string> AstralLoader::ParseLine(std::string line)
{
    std::pair<std::string, std::string> data = std::make_pair("ERROR", "ERROR");

    std::vector<std::string> spl = StringUtils::Split(line, ':');
    if (spl.size() == 0)
        return data;
    else if (spl.size() == 1)
        data.first = StringUtils::Trim(spl[0]);
    else {
        data.first = StringUtils::Trim(spl[0]);
        data.second = StringUtils::Trim(spl[1]);
    }

    return data;
}

void AstralLoader::HandleObject(std::vector<std::string>& lines, size_t& idx, std::string id, std::shared_ptr<AstralObject> parent, bool& isInArray, std::vector<std::variant<int, float, bool, std::string>>& listArray, std::string& listArrayName)
{
    std::shared_ptr<AstralObject> obj = std::make_shared<AstralObject>();
    obj->Id = id;
    
    parent->Children.push_back(obj);

    for (;idx < lines.size(); idx++)
        if (!HandleLineData(lines, idx, obj, isInArray, listArray, listArrayName))
            break;
}

bool AstralLoader::HandleLineData(std::vector<std::string>& lines, size_t& idx, std::shared_ptr<AstralObject> parent, bool& isInArray, std::vector<std::variant<int, float, bool, std::string>>& listArray, std::string& listArrayName)
{
    std::pair<std::string, std::string> line = ParseLine(lines[idx]);

    if (line.second == "}") {
        return false;
    }
    else if (line.second == "{") {
        idx++;
        HandleObject(lines, idx, line.first, parent, isInArray, listArray, listArrayName);
    }
    else if (isInArray) {
        if (line.first == "]") {
            parent->Values.push_back(std::make_shared<AstralValue>(listArrayName, listArray));
            listArray.clear();
            isInArray = false;
        }
        else {
            listArray.push_back(lines[idx]);
        }
    }
    else {
        if (line.second == "[") {
            isInArray = true;
            listArrayName = line.first;
        }
        else {
            parent->Values.push_back(std::make_shared<AstralValue>(line.first, line.second));
        }
    }

    return true;
}

std::shared_ptr<AstralObject> AstralLoader::LoadAstral(std::string src)
{
    std::shared_ptr<AstralObject> root = std::make_shared<AstralObject>();
    root->Id = "ROOT";

    std::vector<std::string> lines;

    std::istringstream lineStream(src);
    std::string line;
    while (std::getline(lineStream, line))
        lines.push_back(StringUtils::Trim(line));

    bool inArray = false;
    std::vector<std::variant<int, float, bool, std::string>> list;
    std::string listArrayName = "";

    for (size_t i = 0; i < lines.size(); i++)
        if (!HandleLineData(lines, i, root, inArray, list, listArrayName))
            break;

    return root;
}
