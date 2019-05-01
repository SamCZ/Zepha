//
// Created by aurailus on 14/04/19.
//

#include "BlockModelEntity.h"

BlockModelEntity::BlockModelEntity(GameDefs& defs) :
    defs(defs) {
    auto m = new Mesh();
    setMesh(m);
}

void BlockModelEntity::setModel(unsigned int blockID, unsigned short crackLevel) {
    if (blockID != this->blockID || crackLevel != this->crackLevel) {
        this->blockID = blockID;
        this->crackLevel = crackLevel;

        auto model = defs.blocks().getBlock(blockID).getModel();
        auto m = new Mesh();

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        unsigned int indOffset = 0;

        crackedFaces.clear();
        std::cout << "Making model" << std::endl;

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
}

void BlockModelEntity::clearModel() {
    auto m = new Mesh();
    setMesh(m);
}

void BlockModelEntity::addFaces(unsigned int &indOffset, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts) {

    for (const MeshPart& mp : meshParts) {
        glm::vec4 uv;
        auto ref = defs.textures().generateCrackImage(mp.texture->name, crackLevel);
        if (ref == nullptr) {
            std::string missing("_missing");
            uv = defs.textures().getTextureRef(missing)->uv;
        }
        uv = ref->uv;

        crackedFaces.push_back(ref);

        for (const MeshVertex &vertex : mp.vertices) {

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

        for (unsigned int index : mp.indices) {
            indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}