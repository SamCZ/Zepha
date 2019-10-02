//
// Created by aurailus on 22/08/19.
//

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "ModelBone.h"
#include "ModelAnimation.h"
#include "../graph/meshtypes/EntityMesh.h"
#include "../../def/texture/TextureAtlas.h"
#include "../../util/Mat4Conv.h"
#include "../../util/Pointer.h"
#include "../../def/block/BlockDef.h"
#include "../../def/craftitem/CraftItemDef.h"

class Model {
public:
    Model() = default;

    void fromMesh(uptr<EntityMesh> mesh);
    void fromBlock(BlockDef& def);
    void fromItem(CraftItemDef& def, TextureAtlas& atlas);
    int  import(const std::string& path, const std::vector<std::shared_ptr<AtlasRef>>& texture);

    void getTransformsByFrame(double frame, std::tuple<int, int> bounds, std::vector<glm::mat4>& transforms);
//    void getTransformsByTime(double time, std::tuple<uint> bounds, std::vector<glm::mat4>& transforms);

    const ModelAnimation& getAnimation();

    std::vector<uptr<EntityMesh>> meshes;
private:
    void loadModelMeshes(aiNode *node, const aiScene *scene);
    void loadMeshAndBone(aiMesh *mesh, uptr<EntityMesh> &target);
    void loadAnimations(const aiScene *scene);

    void calcBoneHeirarchy(aiNode *node, const aiScene *scene, int parentBoneIndex);
    void calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform, std::tuple<int, int> bounds, std::vector<glm::mat4>& transforms);

    void calcInterpolatedPosition(glm::vec3& position, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds);
    void calcInterpolatedRotation(aiQuaternion& rotation, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds);
    void calcInterpolatedScale(glm::vec3& scale, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds);

    unsigned int findPositionIndex(double animTime, AnimChannel& channel);
    unsigned int findRotationIndex(double animTime, AnimChannel& channel);
    unsigned int findScaleIndex(double animTime, AnimChannel& channel);

    ModelAnimation animation {};
    ModelBone* rootBone = nullptr;
    std::vector<ModelBone> bones {};
    std::vector<std::shared_ptr<AtlasRef>> textures {};

    glm::mat4 globalInverseTransform {};
};

