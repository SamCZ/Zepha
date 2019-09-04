//
// Created by aurailus on 14/04/19.
//

#include "BlockCrackEntity.h"


BlockCrackEntity::BlockCrackEntity(LocalDefs &defs, glm::vec3 blockPos, unsigned int blockID) :
    defs(defs),
    blockPos(blockPos),
    blockID(blockID) {
    setPos(blockPos);

    update();
}

void BlockCrackEntity::update() {
    if (targetDamage > damage) damage = static_cast<float>(fmin(targetDamage, damage + 0.2));
    else if (targetDamage < damage) damage = static_cast<float>(fmax(targetDamage, damage - 0.2));

    auto crackLevel = static_cast<unsigned short>(std::max(std::min(static_cast<int>(std::floor(damage * 8)), 7), 0));

    if (crackLevel != this->crackLevel) {
        this->crackLevel = crackLevel;

        auto model = defs.defs().blockFromId(blockID).model;

        std::vector<EntityVertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int indOffset = 0;

        crackedFaces.clear();

        for (int i = 0; i < 7; i++) {
            addFaces(indOffset, vertices, indices, model.parts[i]);
        }

        uptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
        mesh->create(vertices, indices);
        this->model->fromMesh(std::move(mesh));
    }
}

void BlockCrackEntity::setNewDamage(float damage) {
    this->targetDamage = damage;
}

void BlockCrackEntity::addFaces(unsigned int &indOffset, std::vector<EntityVertex> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts) {
    for (const MeshPart& mp : meshParts) {
        glm::vec4 uv;
        auto ref = defs.textures().generateCrackImage(mp.texture->name, static_cast<unsigned short>(crackLevel));
        if (ref == nullptr) {
            std::string missing("_missing");
            uv = defs.textures().getTextureRef(missing)->uv;
        }
        else uv = ref->uv;

        crackedFaces.push_back(ref);

        for (const BlockModelVertex &vertex : mp.vertices) {
            glm::vec3 pushed_pos = vertex.pos;
            pushed_pos += glm::normalize(vertex.nml) * 0.003f;
            glm::vec4 tex = {uv.x + (uv.z - uv.x) * vertex.texUVs.x, uv.y + ((uv.w - uv.y) * vertex.texUVs.y), 0, 0};

            vertices.push_back({
                pushed_pos,
                tex,
                {1, 1, 1},
                true,
                vertex.nml,
                {}, {}
            });
        }

        for (unsigned int index : mp.indices) {
            indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}