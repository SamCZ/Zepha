//
// Created by aurailus on 14/04/19.
//

#include <glm/glm.hpp>

#include "BlockCrackEntity.h"

#include "../../../def/LocalSubgame.h"
#include "../../../def/item/MeshPart.h"
#include "../../../def/item/BlockDef.h"
#include "../../../def/texture/AtlasRef.h"
#include "../../../def/item/BlockModelVertex.h"

BlockCrackEntity::BlockCrackEntity(BlockDef &def, TextureAtlas& textureAtlas, glm::ivec3 pos) :
    def(def),
    textureAtlas(textureAtlas) {
    maxHealth = def.health;
    setPos(pos);
    update();
}

void BlockCrackEntity::update() {
//    if (damagePending > 0) {
//        double amt = std::min(damagePending, 0.2);
//        damage += amt;
//        damagePending -= amt;
//    }
//    else if (damagePending < 0) {
//        double amt = std::max(damagePending, -0.2);
//        damage += amt;
//        damagePending -= amt;
//    }
    damage += damagePending;
    damagePending = 0;

    auto crackLevel = static_cast<unsigned short>(std::max(std::min(static_cast<int>(std::floor((damage / static_cast<float>(maxHealth)) * 8)), 7), 0));

    if (crackLevel != this->crackLevel) {
        this->crackLevel = crackLevel;

        std::vector<EntityVertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int indOffset = 0;

        crackedFaces.clear();
        for (int i = 0; i < 7; i++) addFaces(indOffset, vertices, indices, def.model.parts[i]);

        std::unique_ptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
        mesh->create(vertices, indices);
        this->model->fromMesh(std::move(mesh));
    }
}

void BlockCrackEntity::addDamage(double damage) {
    damagePending += damage;
    update();
}

void BlockCrackEntity::setDamage(double damage) {
    double diff = damage - (this->damage + damagePending);
    std::cout << diff << ", " << this->damage << ", " << damagePending << std::endl;
    addDamage(diff);
}

void BlockCrackEntity::addFaces(unsigned int &indOffset, std::vector<EntityVertex> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts) {
    for (const MeshPart& mp : meshParts) {
        glm::vec4 uv;
        auto ref = textureAtlas.generateCrackImage(mp.texture->name, static_cast<unsigned short>(crackLevel));
        if (ref == nullptr) {
            uv = textureAtlas["_missing"]->uv;
        }
        else uv = ref->uv;

        crackedFaces.push_back(ref);

        for (const BlockModelVertex &vertex : mp.vertices) {
            glm::vec3 pushed_pos = vertex.pos;
            pushed_pos += glm::normalize(vertex.nml) * 0.003f;
            glm::vec4 tex = {uv.x + (uv.z - uv.x) * vertex.texUVs.x, uv.y + ((uv.w - uv.y) * vertex.texUVs.y), 0, 1};

            vertices.push_back({ pushed_pos, tex, {1, 1, 1}, true, vertex.nml, {}, {} });
        }

        for (unsigned int index : mp.indices) indices.push_back(indOffset + index);

        indOffset += mp.vertices.size();
    }
}
