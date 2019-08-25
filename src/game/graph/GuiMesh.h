//
// Created by aurailus on 24/08/19.
//

#pragma once

#include <vector>
#include "Mesh.h"
#include "GuiVertex.h"

class GuiMesh : public Mesh {
public:
    GuiMesh() = default;
    GuiMesh(const GuiMesh& o);
    void create(const std::vector<GuiVertex>& vertices, const std::vector<unsigned int>& indices);
    ~GuiMesh() = default;
};
