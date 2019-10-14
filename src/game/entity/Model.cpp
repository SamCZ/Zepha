
//
// Created by aurailus on 22/08/19.
//

#include "Model.h"

void Model::fromMesh(uptr<EntityMesh> mesh) {
    meshes.clear();
    meshes.push_back(std::move(mesh));
}

int Model::fromFile(const std::string &path, const std::vector<std::shared_ptr<AtlasRef>> &textures) {
    this->textures = textures;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << Log::err << "ERROR::ASSIMP::" << importer.GetErrorString() << Log::endl;
        return 1;
    }

    loadModelMeshes(scene->mRootNode, scene);
    loadAnimations(scene);

    calcBoneHeirarchy(scene->mRootNode, scene, -1);

    globalInverseTransform = glm::inverse(MatConv::AiToGLMMat4(scene->mRootNode->mTransformation));

    return 0;
}

int Model::fromSerialized(const SerializedModel& model, const std::vector<std::shared_ptr<AtlasRef>>& textures) {
    this->textures = textures;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(model.data.data(), model.data.length(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals, "B3D");

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << Log::err << "ERROR::ASSIMP::" << importer.GetErrorString() << Log::endl;
        return 1;
    }

    loadModelMeshes(scene->mRootNode, scene);
    loadAnimations(scene);

    calcBoneHeirarchy(scene->mRootNode, scene, -1);
    globalInverseTransform = glm::inverse(MatConv::AiToGLMMat4(scene->mRootNode->mTransformation));
    return 0;
}

void Model::getTransformsByFrame(double frame, std::tuple<int, int> bounds, std::vector<glm::mat4>& transforms) {
    transforms.resize(bones.size());
    if (rootBone) calcBoneTransformation(frame, *rootBone, glm::mat4(1.0f), bounds, transforms);
}

//void Model::getTransformsByTime(double time, std::vector<glm::mat4>& transforms) {
//    double tps = animation.ticksPerSecond;
//    double frameTime = fmod(time * tps, animation.duration);
//
//    getTransformsByFrame(frameTime, transforms);
//}

const ModelAnimation &Model::getAnimation() {
    return animation;
}

void Model::loadModelMeshes(aiNode *node, const aiScene *scene) {
    for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(std::make_unique<EntityMesh>());
        loadMeshAndBone(mesh, meshes[i]);
    }

    for (uint i = 0; i < node->mNumChildren; i++) {
        loadModelMeshes(node->mChildren[i], scene); //Recurse down
    }
}

void Model::loadMeshAndBone(aiMesh *mesh, uptr<EntityMesh>& target) {
    std::vector<EntityVertex> vertices;
    std::vector<unsigned int> indices;

    //Process Vertices
    for (uint i = 0; i < mesh->mNumVertices; i++) {
        EntityVertex vertex {};

        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        vertex.colorBlend = {1, 1, 1};

        assert(mesh->mMaterialIndex >= 0 && mesh->mMaterialIndex < textures.size());
        auto& texture = textures[mesh->mMaterialIndex];

        if (mesh->mTextureCoords[0]) {
            //Set texture coordinates
            vertex.useTex = true;
            vertex.colorData = {
                texture->uv.x + mesh->mTextureCoords[0][i].x * (texture->uv.z - texture->uv.x),
                texture->uv.y + mesh->mTextureCoords[0][i].y * (texture->uv.w - texture->uv.y), 0, 0
            };
        }
        else vertex.colorData = {1, 1, 1, 1};

        vertices.push_back(vertex);
    }

    //Process Indices
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //Process Mesh Bones and add to bone list
    bones.resize(mesh->mNumBones);
    for (uint i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];

        bones[i] = ModelBone(static_cast<uint>(i), -1, {bone->mName.data});
        bones[i].offsetMatrix = glm::transpose(MatConv::AiToGLMMat4(bone->mOffsetMatrix));

        for (uint j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight* weight = &bone->mWeights[j];
            if (weight->mVertexId >= vertices.size()) assert(0);

            uint bid = 0;
            while (vertices[weight->mVertexId].boneWeights[bid] != 0) {
                bid++;
                assert(bid < 4);
            }

            vertices[weight->mVertexId].boneIDs[bid] = i;
            vertices[weight->mVertexId].boneWeights[bid] = weight->mWeight;
        }
    }

    //Create mesh
    target->create(vertices, indices);
}

void Model::loadAnimations(const aiScene *scene) {
    assert(scene->mNumAnimations <= 1);

    if (scene->mNumAnimations == 1) {
        const aiAnimation* aiAnim = scene->mAnimations[0];

        animation = ModelAnimation(aiAnim->mName.data);

        animation.duration = static_cast<uint>(aiAnim->mDuration);
        animation.ticksPerSecond = aiAnim->mTicksPerSecond;

        animation.channels.resize(bones.size());

        for (unsigned int j = 0; j < aiAnim->mNumChannels; j++) {
            const aiNodeAnim* aiChannel = aiAnim->mChannels[j];

            int index = -1;
            for (unsigned int k = 0; k < bones.size(); k++) {
                if (std::string {aiChannel->mNodeName.data} == bones[k].name) {
                    index = k;
                    continue;
                }
            }

            if (index == -1) {
//                std::cout << aiChannel->mNodeName.data << std::endl;
            }
            else {
                animation.channels[index] = AnimChannel(static_cast<unsigned int>(index), aiChannel->mNodeName.data);
                AnimChannel &channel = animation.channels[index];

                //Copy Rotation Keys
                channel.rotationKeys.reserve(aiChannel->mNumRotationKeys);
                for (unsigned int k = 0; k < aiChannel->mNumRotationKeys; k++) {
                    aiQuatKey *key = &aiChannel->mRotationKeys[k];
                    channel.rotationKeys.emplace_back(key->mTime, key->mValue);
                }

                //Copy Position Keys
                channel.positionKeys.reserve(aiChannel->mNumPositionKeys);
                for (unsigned int k = 0; k < aiChannel->mNumPositionKeys; k++) {
                    aiVectorKey *key = &aiChannel->mPositionKeys[k];
                    channel.positionKeys.emplace_back(key->mTime,
                                                      glm::vec3{key->mValue.x, key->mValue.y, key->mValue.z});
                }

                //Copy Scale Keys
                channel.scaleKeys.reserve(aiChannel->mNumScalingKeys);
                for (unsigned int k = 0; k < aiChannel->mNumScalingKeys; k++) {
                    aiVectorKey *key = &aiChannel->mScalingKeys[k];
                    channel.scaleKeys.emplace_back(key->mTime, glm::vec3{key->mValue.x, key->mValue.y, key->mValue.z});
                }
            }
        }
    }
}

void Model::calcBoneHeirarchy(aiNode *node, const aiScene *scene, int parentBoneIndex) {
    int index = -1;

    for (auto &bone : bones) {
        if (bone.name == std::string(node->mName.data)) {
            bone.parent = parentBoneIndex;
            index = bone.index;
            if (parentBoneIndex == -1) {
                rootBone = &bone;
                std::cout << bone.name << std::endl;
            }
            else bones[bone.parent].children.push_back(&bone);
            break;
        }
    }

    for (uint i = 0; i < node->mNumChildren; i++) {
        calcBoneHeirarchy(node->mChildren[i], scene, index);
    }
}

void Model::calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform, std::tuple<int, int> bounds, std::vector<glm::mat4>& transforms) {
    AnimChannel* channel = nullptr;

    for (auto &i : animation.channels) {
        if (i.index == bone.index) channel = &i;
    }

    glm::mat4 boneTransformation(1.0f);

    if (channel) {
        glm::vec3 scale;
        calcInterpolatedScale(scale, animTime, bone, *channel, bounds);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), scale);

        aiQuaternion rotation;
        calcInterpolatedRotation(rotation, animTime, bone, *channel, bounds);
        glm::mat4 rotationMat = glm::transpose(MatConv::AiToGLMMat3(rotation.GetMatrix()));

        glm::vec3 position;
        calcInterpolatedPosition(position, animTime, bone, *channel, bounds);
        glm::mat4 positionMat = glm::translate(glm::mat4(1.0), position);

        boneTransformation = positionMat * rotationMat * scaleMat;
    }

    glm::mat4 globalTransformation = parentTransform * boneTransformation;
    transforms[bone.index] = globalInverseTransform * globalTransformation * bone.offsetMatrix;

    for (auto& child : bone.children) {
        calcBoneTransformation(animTime, *child, globalTransformation, bounds, transforms);
    }
}

void Model::calcInterpolatedPosition(glm::vec3 &position, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds) {
    if (channel.positionKeys.empty()) { position = glm::vec3(0); return; }
    if (channel.positionKeys.size() == 1) { position = channel.positionKeys[0].second; return; }

    uint index = findPositionIndex(animTime, channel);
    uint nextIndex = index + 1;
    assert(nextIndex < channel.positionKeys.size());

    double delta = channel.positionKeys[nextIndex].first - channel.positionKeys[index].first;
    double factor = (animTime - channel.positionKeys[index].first) / delta;
    if (nextIndex >= std::get<1>(bounds)) factor = 0;
    assert(factor >= 0 && factor <= 1);

    glm::vec3 startPosition = channel.positionKeys[index].second;
    glm::vec3 endPosition = channel.positionKeys[nextIndex].second;

    position = glm::mix(startPosition, endPosition, factor);
}

void Model::calcInterpolatedRotation(aiQuaternion &rotation, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds) {
    if (channel.rotationKeys.empty()) { return; }
    if (channel.rotationKeys.size() == 1) { rotation = channel.rotationKeys[0].second; return; }

    uint index = findRotationIndex(animTime, channel);
    uint nextIndex = index + 1;
    assert(nextIndex < channel.rotationKeys.size());

    double delta = channel.rotationKeys[nextIndex].first - channel.rotationKeys[index].first;
    double factor = (animTime - channel.rotationKeys[index].first) / delta;
    if (nextIndex >= std::get<1>(bounds)) factor = 0;
    assert(factor >= 0 && factor <= 1);

    const aiQuaternion& startRotation = channel.rotationKeys[index].second;
    const aiQuaternion& endRotation = channel.rotationKeys[nextIndex].second;

    aiQuaternion::Interpolate(rotation, startRotation, endRotation, static_cast<ai_real>(factor));
    rotation = rotation.Normalize();
}

void Model::calcInterpolatedScale(glm::vec3 &scale, double animTime, ModelBone& bone, AnimChannel& channel, std::tuple<int, int> bounds) {
    if (channel.scaleKeys.empty()) { scale = glm::vec3(1); return; }
    if (channel.scaleKeys.size() == 1) { scale = channel.scaleKeys[0].second; return; }

    uint index = findScaleIndex(animTime, channel);
    uint nextIndex = index + 1;
    assert(nextIndex < channel.scaleKeys.size());

    double delta = channel.scaleKeys[nextIndex].first - channel.scaleKeys[index].first;
    double factor = (animTime - channel.scaleKeys[index].first) / delta;
    if (nextIndex >= std::get<1>(bounds)) factor = 0;
    assert(factor >= 0 && factor <= 1);

    glm::vec3 startScale = channel.scaleKeys[index].second;
    glm::vec3 endScale = channel.scaleKeys[nextIndex].second;

    scale = glm::mix(startScale, endScale, factor);
}

uint Model::findPositionIndex(double animTime, AnimChannel &channel) {
    for (uint i = 1; i < channel.positionKeys.size(); i++) {
        if (channel.positionKeys[i].first > animTime) return i - 1;
    }
    assert(false);
}

uint Model::findRotationIndex(double animTime, AnimChannel &channel) {
    for (uint i = 1; i < channel.rotationKeys.size(); i++) {
        if (channel.rotationKeys[i].first > animTime) return i - 1;
    }
    assert(false);
}

uint Model::findScaleIndex(double animTime, AnimChannel &channel) {
    for (uint i = 1; i < channel.scaleKeys.size(); i++) {
        if (channel.scaleKeys[i].first > animTime) return i - 1;
    }
    assert(false);
}