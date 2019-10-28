//
// Created by aurailus on 24/08/19.
//

#include "GuiMesh.h"

GuiMesh::GuiMesh(const GuiMesh &o) {
    throw "Copy constructor for GuiMesh is not supported! Throwing.";
}

void GuiMesh::create(const std::vector<GuiVertex>& vertices, const std::vector<unsigned int>& indices) {
    if (vertices.size() == 0) return;

    indCount = static_cast<GLsizei>(indices.size());

    genArrays(static_cast<unsigned int>(vertices.size() * sizeof(GuiVertex)),
              static_cast<unsigned int>(indices.size() * sizeof(unsigned int)),
              &vertices.front(), &indices.front());

    unsigned int idx = 0;
    createVertexAttrib(idx++, 2, GL_FLOAT, STRIDE_OFFSET_GUI(position));
    createVertexAttrib(idx++, 4, GL_FLOAT, STRIDE_OFFSET_GUI(colorData));
    createVertexAttrib(idx++, 3, GL_FLOAT, STRIDE_OFFSET_GUI(colorBlend));
    createVertexAttrib(idx  , 1, GL_FLOAT, STRIDE_OFFSET_GUI(useTexture));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}