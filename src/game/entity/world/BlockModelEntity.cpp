//
// Created by aurailus on 14/04/19.
//

#include "BlockModelEntity.h"

BlockModelEntity::BlockModelEntity(DynamicAtlas* texAtlas) {
    this->texAtlas = texAtlas;
    auto m = new Mesh();
    setMesh(m);
}

void BlockModelEntity::setModel(BlockModel &model) {
    auto m = new Mesh();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int indOffset = 0;

    addFaces(indOffset, vertices, indices, model.leftFaces);
    addFaces(indOffset, vertices, indices, model.rightFaces);
    addFaces(indOffset, vertices, indices, model.topFaces);
    addFaces(indOffset, vertices, indices, model.bottomFaces);
    addFaces(indOffset, vertices, indices, model.frontFaces);
    addFaces(indOffset, vertices, indices, model.backFaces);
    addFaces(indOffset, vertices, indices, model.noCulledFaces);

    m->create(&vertices, &indices);
    setMesh(m);
}

void BlockModelEntity::clearModel() {
    auto m = new Mesh();
    setMesh(m);
}

void BlockModelEntity::addFaces(unsigned int &indOffset, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart *> &meshParts) {
    std::string tex("default_crack_" + to_string((int)round(n)));

    n += 0.005f;
    if (n > 7) n = 0;

    auto uv = texAtlas->getTextureUVs(tex);

    for (MeshPart *mp : meshParts) {

        for (MeshVertex &vertex : mp->vertices) {

            glm::vec3 pushed_pos = vertex.pos;
            pushed_pos += glm::normalize(vertex.nml) * 0.0005f;

            vertices.push_back(pushed_pos.x);
            vertices.push_back(pushed_pos.y);
            vertices.push_back(pushed_pos.z);

            vertices.push_back(1);

            vertices.push_back(uv.x + ((uv.z - uv.x) * vertex.texUVs.x));
            vertices.push_back(uv.y + ((uv.w - uv.y) * vertex.texUVs.y));
            vertices.push_back(0);
            vertices.push_back(0);

            vertices.push_back(vertex.nml.x);
            vertices.push_back(vertex.nml.y);
            vertices.push_back(vertex.nml.z);
        }

        for (unsigned int index : mp->indices) {
            indices.push_back(indOffset + index);
        }

        indOffset += mp->vertices.size();
    }
}