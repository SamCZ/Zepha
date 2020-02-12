//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"

BlockDef::BlockDef(const std::string &identifier, const std::string& name, unsigned short maxStackSize,
        const BlockModel &model, bool solid, const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes) :
    BlockDef(identifier, 0, name, maxStackSize, model, solid, sBoxes, cBoxes) {}

BlockDef::BlockDef(const std::string& identifier, unsigned int index, const std::string& name,
        unsigned short maxStackSize, const BlockModel& model, bool solid,
        const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes) :

    ItemDef {identifier, name, index, maxStackSize, ItemDef::Type::BLOCK},
    model(model),
    culls(model.culls),
    solid(solid),
    sBoxes(sBoxes),
    cBoxes(cBoxes) {}

void BlockDef::createModel() {
    std::unique_ptr<EntityMesh> entity = std::make_unique<EntityMesh>();
    unsigned int indOffset = 0;

    std::vector<EntityVertex> entityVertices;
//    std::vector<GuiVertex> guiVertices;
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
//                float brightness = 0.8 + (glm::abs(vertex.nml.z) * 0.2) + (vertex.nml.y * 0.4);
//                guiVertices.push_back(GuiVertex {
//                    vertex.pos - glm::vec3(0.5f),
//                    {vertex.tex.x, vertex.tex.y, 0, 1},
//                    glm::vec3(brightness),
//                    true
//                });
            }

            for (unsigned int index : p.indices) {
                indices.push_back(indOffset + index);
            }

            indOffset += p.vertices.size();
        }
    }

    entity->create(entityVertices, indices);
    entityModel = std::make_shared<Model>();
    entityModel->fromMesh(std::move(entity));

//    guiModel->create(guiVertices, indices);
}
