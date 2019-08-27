//
// Created by aurailus on 22/08/19.
//

#pragma once

#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "ModelBone.h"
#include "../texture/TextureAtlas.h"
#include "../../util/Mat4Conv.h"
#include "../../game/graph/EntityMesh.h"
#include "ModelAnimation.h"

class Model {
public:
    Model() = default;
    int create(const std::string& path, std::shared_ptr<AtlasRef> texture);

    void getTransforms(double time, std::vector<glm::mat4>& transforms);

    std::vector<EntityMesh> meshes;
private:
    void loadModelMeshes(aiNode *node, const aiScene *scene);
    void loadMeshAndBone(aiMesh *mesh, const aiScene *scene, EntityMesh& target);
    void loadAnimations(const aiScene *scene);

    void calcBoneHeirarchy(aiNode *node, const aiScene *scene, int parentBoneIndex);
    void calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform);

    void calcInterpolatedPosition(glm::vec3& position, double animTime, ModelBone bone, AnimChannel& channel);
    void calcInterpolatedRotation(aiQuaternion& rotation, double animTime, ModelBone bone, AnimChannel& channel);
    void calcInterpolatedScale(glm::vec3& scale, double animTime, ModelBone bone, AnimChannel& channel);

    unsigned int findPositionIndex(double animTime, AnimChannel& channel);
    unsigned int findRotationIndex(double animTime, AnimChannel& channel);
    unsigned int findScaleIndex(double animTime, AnimChannel& channel);

    ModelBone* rootBone = nullptr;
    std::vector<ModelBone> bones {};
    std::vector<ModelAnimation> animations {};

    glm::mat4 globalInverseTransform {};

    std::shared_ptr<AtlasRef> texture = nullptr;
};

