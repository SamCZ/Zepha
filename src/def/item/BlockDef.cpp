//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"

BlockDef::BlockDef(
    const std::string& identifier,
    const std::string& name,
    unsigned short maxStackSize,
    const BlockModel& model,
    const BlockModel& farModel,
    bool solid,
    glm::ivec3 lightSource,
    bool lightPropagates,
    const std::vector<SelectionBox>& sBoxes,
    const std::vector<SelectionBox>& cBoxes,
    unsigned int index) :

    ItemDef {identifier, name, index, maxStackSize, ItemDef::Type::BLOCK},

    model(model),
    farModel(farModel),
    culls(model.culls),
    solid(solid),
    lightSource(lightSource),
    lightPropagates(lightPropagates),
    sBoxes(sBoxes),
    cBoxes(cBoxes) {}

void BlockDef::createModel() {
    std::unique_ptr<EntityMesh> entity = std::make_unique<EntityMesh>();
    unsigned int indOffset = 0;

    std::vector<EntityVertex> entityVertices;
    std::vector<unsigned int> indices;

    for (std::vector<MeshPart>& pArray : model.parts) {
        for (MeshPart& p : pArray) {
            for (const BlockModelVertex &vertex : p.vertices) {
                entityVertices.push_back(EntityVertex {
                    vertex.pos - glm::vec3(0.5f),
                    {vertex.tex.x, vertex.tex.y, 0, 1},
                    {1, 1, 1},
                    true,
                    vertex.nml,
                    {}, {}
                });
            }

            for (unsigned int index : p.indices) indices.push_back(indOffset + index);
            indOffset += p.vertices.size();
        }
    }

    entity->create(entityVertices, indices);
    entityModel = std::make_shared<Model>();
    entityModel->fromMesh(std::move(entity));
}
