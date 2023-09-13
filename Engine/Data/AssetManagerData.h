#pragma once

#include "VarData.h"

#include <string>

#include "Basics/Initializable.h"

struct AssetManagerData : public InitializableBasic {
public:
	std::u16string SourceDir;
	bool IsPackaged = false;
};