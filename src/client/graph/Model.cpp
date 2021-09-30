
//
// Created by aurailus on 22/08/19.
//

#include <iostream>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

#include "util/Log.h"
#include "util/Mat4Conv.h"
#include "game/atlas/asset/AtlasTexture.h"
#include "game/atlas/asset/SerializedModel.h"

void Model::fromMesh(std::unique_ptr<EntityMesh> mesh) {
	meshes.clear();
	meshes.push_back(std::move(mesh));
}

int Model::fromFile(const std::string& path, const std::vector<AtlasTexture>& textures) {
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

int Model::fromSerialized(const SerializedModel& model, const std::vector<AtlasTexture>& textures) {
	this->textures = textures;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(model.data.data(), model.data.length(),
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals, "B3D");
	
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

void Model::getTransformsByFrame(double frame, glm::ivec2 bounds, std::vector<glm::mat4>& transforms) {
	transforms.resize(bones.size());
	
	if (!rootBones.empty())
		for (auto bone : rootBones)
			calcBoneTransformation(frame, *bone, glm::mat4(1.0f), bounds, transforms);
}

//void Model::getTransformsByTime(double time, std::vector<glm::mat4>& transforms) {
//    double tps = animation.ticksPerSecond;
//    double frameTime = fmod(time * tps, animation.duration);
//
//    getTransformsByFrame(frameTime, transforms);
//}

const ModelAnimation& Model::getAnimation() {
	return animation;
}

void Model::loadModelMeshes(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(std::make_unique<EntityMesh>());
		loadMeshAndBone(mesh, meshes[i]);
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		loadModelMeshes(node->mChildren[i], scene); //Recurse down
	}
}

void Model::loadMeshAndBone(aiMesh* mesh, std::unique_ptr<EntityMesh>& target) {
	std::vector<EntityVertex> vertices;
	std::vector<unsigned int> indices;
	
	//Process Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		EntityVertex vertex{};
		
		vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		vertex.colorBlend = { 1, 1, 1 };
		
		assert(mesh->mMaterialIndex >= 0 && mesh->mMaterialIndex < textures.size());
		
		if (mesh->mTextureCoords[0]) {
			auto& texture = textures[mesh->mMaterialIndex];
			
			//Set texture coordinates
			vec4 uvs = texture.getUVPos();
			vertex.useTex = true;
			vertex.colorData = {
				uvs.x + mesh->mTextureCoords[0][i].x * (uvs.z - uvs.x),
				uvs.y + mesh->mTextureCoords[0][i].y * (uvs.w - uvs.y), 0, 1 //Alpha
			};
		}
		else vertex.colorData = { 1, 1, 1, 1 };
		
		vertices.push_back(vertex);
	}
	
	//Process Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	
	//Process Mesh Bones and add to bone list
	bones.resize(mesh->mNumBones);
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];
		
		bones[i] = ModelBone(static_cast<unsigned int>(i), -1, { bone->mName.data });
		bones[i].offsetMatrix = glm::transpose(MatConv::AiToGLMMat4(bone->mOffsetMatrix));
		
		for (unsigned int j = 0; j < bone->mNumWeights; j++) {
			aiVertexWeight* weight = &bone->mWeights[j];
			if (weight->mVertexId >= vertices.size()) assert(0);
			
			unsigned int bid = 0;
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

void Model::loadAnimations(const aiScene* scene) {
	assert(scene->mNumAnimations <= 1);
	
	if (scene->mNumAnimations == 1) {
		const aiAnimation* aiAnim = scene->mAnimations[0];
		
		animation = ModelAnimation(aiAnim->mName.data);
		
		animation.duration = static_cast<unsigned int>(aiAnim->mDuration);
		animation.ticksPerSecond = aiAnim->mTicksPerSecond;
		
		animation.channels.resize(bones.size());
		
		for (unsigned int j = 0; j < aiAnim->mNumChannels; j++) {
			const aiNodeAnim* aiChannel = aiAnim->mChannels[j];
			
			int index = -1;
			for (unsigned int k = 0; k < bones.size(); k++) {
				if (std::string{ aiChannel->mNodeName.data } == bones[k].name) {
					index = k;
					continue;
				}
			}
			
			if (index == -1) {
//                std::cout << aiChannel->mNodeName.data << std::endl;
			}
			else {
				animation.channels[index] = AnimChannel(static_cast<unsigned int>(index), aiChannel->mNodeName.data);
				AnimChannel& channel = animation.channels[index];
				
				//Copy Rotation Keys
				channel.rotationKeys.reserve(aiChannel->mNumRotationKeys);
				for (unsigned int k = 0; k < aiChannel->mNumRotationKeys; k++) {
					aiQuatKey* key = &aiChannel->mRotationKeys[k];
					channel.rotationKeys.emplace_back(key->mTime, key->mValue);
				}
				
				//Copy Position Keys
				channel.positionKeys.reserve(aiChannel->mNumPositionKeys);
				for (unsigned int k = 0; k < aiChannel->mNumPositionKeys; k++) {
					aiVectorKey* key = &aiChannel->mPositionKeys[k];
					channel.positionKeys.emplace_back(key->mTime,
						glm::vec3{ key->mValue.x, key->mValue.y, key->mValue.z });
				}
				
				//Copy Scale Keys
				channel.scaleKeys.reserve(aiChannel->mNumScalingKeys);
				for (unsigned int k = 0; k < aiChannel->mNumScalingKeys; k++) {
					aiVectorKey* key = &aiChannel->mScalingKeys[k];
					channel.scaleKeys.emplace_back(key->mTime,
						glm::vec3{ key->mValue.x, key->mValue.y, key->mValue.z });
				}
			}
		}
	}
}

void Model::calcBoneHeirarchy(aiNode* node, const aiScene* scene, int parentBoneIndex) {
	int index = -1;
	
	for (auto& bone : bones) {
		if (bone.name == std::string(node->mName.data)) {
			bone.parent = parentBoneIndex;
			index = bone.index;
			if (parentBoneIndex == -1) rootBones.push_back(&bone);
			else bones[bone.parent].children.push_back(&bone);
			break;
		}
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++) calcBoneHeirarchy(node->mChildren[i], scene, index);
}

void Model::calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform, glm::ivec2 bounds,
	std::vector<glm::mat4>& transforms) {
	AnimChannel* channel = nullptr;
	for (auto& i : animation.channels) {
		if (i.index == bone.index) {
			channel = &i;
			break;
		}
	}
	
	glm::mat4 boneTransformation(1.0f);
	if (channel) {
		glm::mat4 position = glm::translate(glm::mat4(1.0),
			calcBoneVal<glm::vec3>(animTime, bounds, channel->positionKeys, {},
				[](const glm::vec3& a, const glm::vec3& b, float factor) { return glm::mix(a, b, factor); }));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), calcBoneVal<glm::vec3>(animTime, bounds, channel->scaleKeys, {},
			[](const glm::vec3& a, const glm::vec3& b, float factor) { return glm::mix(a, b, factor); }));
		glm::mat4 rotation = glm::transpose(glm::mat4(MatConv::AiToGLMMat3(
			calcBoneVal<aiQuaternion>(animTime, bounds, channel->rotationKeys, {},
				[](const aiQuaternion& a, const aiQuaternion& b, float factor) {
					aiQuaternion result;
					aiQuaternion::Interpolate(result, a, b, factor);
					return result.Normalize();
				}).GetMatrix())));
		
		boneTransformation = position * rotation * scale;
	}
	
	glm::mat4 globalTransformation = parentTransform * boneTransformation;
	transforms[bone.index] = globalInverseTransform * globalTransformation * bone.offsetMatrix;
	
	for (auto& child : bone.children)
		calcBoneTransformation(animTime, *child, globalTransformation, bounds, transforms);
}