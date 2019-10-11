//
// Created by aurailus on 09/10/19.
//

#pragma once

#include <map>
#include "SerializedModel.h"

class ModelStore {
public:
    std::map<std::string, SerializedModel> models;
};
