//
// Created by aurailus on 22/08/19.
//

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>

#include "ModelBone.h"
#include "ModelAnimation.h"
#include "client/graph/mesh/EntityMesh.h"
#include "game/atlas/asset/AtlasTexture.h"

class SerializedModel;

class Model {
public:
	Model() = default;
	
	void fromMesh(std::unique_ptr<EntityMesh> mesh);
	
	int fromFile(const std::string& path, const std::vector<AtlasTexture>& texture);
	
	int fromSerialized(const SerializedModel& model, const std::vector<AtlasTexture>& texture);
	
	void getTransformsByFrame(double frame, glm::ivec2 bounds, std::vector<glm::mat4>& transforms);
//    void getTransformsByTime(double time, std::tuple<uint> bounds, std::vector<glm::mat4>& transforms);
	
	const ModelAnimation& getAnimation();
	
	std::vector<std::unique_ptr<EntityMesh>> meshes {};

private:
	void loadModelMeshes(aiNode* node, const aiScene* scene);
	
	void loadMeshAndBone(aiMesh* mesh, std::unique_ptr<EntityMesh>& target);
	
	void loadAnimations(const aiScene* scene);
	
	void calcBoneHeirarchy(aiNode* node, const aiScene* scene, int parentBoneIndex);
	
	void calcBoneTransformation(double animTime, ModelBone& bone, glm::mat4 parentTransform, glm::ivec2 bounds,
		std::vector<glm::mat4>& transforms);
	
	template<typename T>
	static inline T calcBoneVal(
		double animTime, glm::ivec2 bounds, const std::vector<std::pair<double, T>>& keysArray,
		const T& def, std::function<T(const T& a, const T& b, float factor)> merge) {
		
		if (keysArray.empty()) return def;
		if (keysArray.size() == 1) return keysArray[0].second;
		
		unsigned int index = 0;
		for (unsigned int i = 1; i < keysArray.size(); i++) {
			if (keysArray[i].first > animTime) {
				index = i - 1;
				break;
			}
		}
		float factor = 1;
		unsigned int nextIndex = index + 1;
		if (nextIndex >= keysArray.size() || nextIndex > bounds.y) nextIndex = bounds.x;
		else {
			double delta = keysArray[nextIndex].first - keysArray[index].first;
			factor = (animTime - keysArray[index].first) / delta;
		}
		
		return merge(keysArray[index].second, keysArray[nextIndex].second, factor);
	}
	
	ModelAnimation animation {};
	std::vector<ModelBone*> rootBones {};
	std::vector<ModelBone> bones {};
	std::vector<AtlasTexture> textures {};
	
	glm::mat4 globalInverseTransform {};
};

