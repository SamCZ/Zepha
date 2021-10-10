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

class AtlasTexture;
class SerializedModel;

class Model {
public:
	Model() = default;
	
	void fromMesh(uptr<EntityMesh> mesh);
	
	u8 fromFile(const string& path, const vec<sptr<AtlasTexture>>& texture);
	
	u8 fromSerialized(const SerializedModel& model, const vec<sptr<AtlasTexture>>& texture);
	
	void getTransformsByFrame(f64 frame, ivec2 bounds, vec<glm::mat4>& transforms);
//    void getTransformsByTime(f64 time, std::tuple<uint> bounds, vec<glm::mat4>& transforms);
	
	const ModelAnimation& getAnimation();
	
	vec<uptr<EntityMesh>> meshes {};

private:
	void loadModelMeshes(aiNode* node, const aiScene* scene);
	
	void loadMeshAndBone(aiMesh* mesh, uptr<EntityMesh>& target);
	
	void loadAnimations(const aiScene* scene);
	
	void calcBoneHeirarchy(aiNode* node, const aiScene* scene, u32 parentBoneIndex);
	
	void calcBoneTransformation(f64 animTime, ModelBone& bone,
		glm::mat4 parentTransform, ivec2 bounds, vec<glm::mat4>& transforms);
	
	template<typename T>
	static inline T calcBoneVal(f64 animTime, ivec2 bounds, const vec<std::pair<f64, T>>& keysArray,
		const T& def, std::function<T(const T& a, const T& b, f32)> merge) {
		
		if (keysArray.empty()) return def;
		if (keysArray.size() == 1) return keysArray[0].second;
		
		u32 index = 0;
		for (u32 i = 1; i < keysArray.size(); i++) {
			if (keysArray[i].first > animTime) {
				index = i - 1;
				break;
			}
		}
		f32 factor = 1;
		u32 nextIndex = index + 1;
		if (nextIndex >= keysArray.size() || nextIndex > bounds.y) nextIndex = bounds.x;
		else {
			f64 delta = keysArray[nextIndex].first - keysArray[index].first;
			factor = (animTime - keysArray[index].first) / delta;
		}
		
		return merge(keysArray[index].second, keysArray[nextIndex].second, factor);
	}
	
	ModelAnimation animation {};
	vec<ModelBone*> rootBones {};
	vec<ModelBone> bones {};
	vec<sptr<AtlasTexture>> textures {};
	
	glm::mat4 globalInverseTransform {};
};

