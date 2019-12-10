//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"


BlockDef::BlockDef(const std::string &identifier, const std::string& name, const BlockModel &model, bool solid, const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes) :
    BlockDef(identifier, 0, name, model, solid, sBoxes, cBoxes) {}

BlockDef::BlockDef(const std::string& identifier, unsigned int index, const std::string& name, const BlockModel& model, bool solid, const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes) :
    ItemDef {identifier, name, index, ItemDef::Type::BLOCK},
    model(model),
    culls(model.culls),
    solid(solid),
    sBoxes(sBoxes),
    cBoxes(cBoxes) {}

void BlockDef::createModel() {
    uptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
    uint indOffset = 0;

    std::vector<EntityVertex> vertices;
    std::vector<unsigned int> indices;

    for (std::vector<MeshPart>& pArray : model.parts) {
        for (MeshPart& p : pArray) {
            for (const BlockModelVertex &vertex : p.vertices) {
                vertices.push_back(EntityVertex {
                        vertex.pos - glm::vec3(0.5f),
                        {vertex.tex.x, vertex.tex.y, 0, 0},
                        {1, 1, 1},
                        true,
                        vertex.nml,
                        {}, {}
                });
            }

            for (unsigned int index : p.indices) {
                indices.push_back(indOffset + index);
            }

            indOffset += p.vertices.size();
        }
    }

    mesh->create(vertices, indices);
    entityModel->fromMesh(std::move(mesh));
}
