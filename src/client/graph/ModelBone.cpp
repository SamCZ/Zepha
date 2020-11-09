//
// Created by aurailus on 25/08/19.
//

#include "ModelBone.h"

ModelBone::ModelBone(unsigned int index, int parent, const std::string& name) :
	index(index),
	parent(parent),
	name(name) {}