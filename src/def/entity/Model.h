//
// Created by aurailus on 22/08/19.
//

#ifndef ZEUS_MODEL_H
#define ZEUS_MODEL_H

#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../game/graph/EntityMesh.h"
#include "../texture/TextureAtlas.h"

class Model {
public:
    Model() = default;
    int create(const std::string& path, TextureAtlas& atlas, const std::string& tex);

    std::vector<EntityMesh> meshes;
private:
    void processNode(aiNode* node, const aiScene* scene);
    EntityMesh processMesh(aiMesh* mesh, const aiScene* scene);

    TextureAtlas* atlas = nullptr;
    std::string tex = "";
};


#endif //ZEUS_MODEL_H
