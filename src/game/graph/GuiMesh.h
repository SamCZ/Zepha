//
// Created by aurailus on 24/08/19.
//

#ifndef ZEUS_GUIMESH_H
#define ZEUS_GUIMESH_H

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

#endif //ZEUS_GUIMESH_H
