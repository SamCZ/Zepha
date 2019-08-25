//
// Created by aurailus on 22/08/19.
//

#include "Model.h"

int Model::create(const std::string &path, TextureAtlas& atlas, const std::string& tex) {
    this->atlas = &atlas;
    this->tex = tex;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return 1;
    }

    processNode(scene->mRootNode, scene);
    return 0;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    std::cout << node->mName.data << std::endl;
    for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (uint i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

EntityMesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    EntityMesh m;

    std::vector<EntityVertex> vertices;
    std::vector<unsigned int> indices;

    auto texRef = atlas->getTextureRef(tex);

    for (uint i = 0; i < mesh->mNumVertices; i++) {
        EntityVertex vertex {};
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        vertex.colorBlend = {1, 1, 1};
        if (mesh->mTextureCoords[0]) {
            vertex.useTex = true;
            vertex.colorData.x = texRef->uv.x + mesh->mTextureCoords[0][i].x * (texRef->uv.z - texRef->uv.x);
            vertex.colorData.y = texRef->uv.y + mesh->mTextureCoords[0][i].y * (texRef->uv.w - texRef->uv.y);
        }
        else {
            vertex.colorData = {1, 0, 0, 1};
        }
        vertices.push_back(vertex);
    }

    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        //TODO: Require list of textures to use with the model, and use those in here to translate texCoords based on idx
    }

    m.create(vertices, indices);
    return m;
}