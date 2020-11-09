//
// Created by aurailus on 09/04/19.
//

#include "DrawableGroup.h"

void DrawableGroup::draw(Renderer& renderer) {
	for (auto drawable : children) {
		drawable->draw(renderer);
	}
}

void DrawableGroup::addDrawable(Drawable* drawable) {
	children.push_back(drawable);
}

void DrawableGroup::removeDrawable(Drawable* drawable) {
	for (auto it = children.begin(); it < children.end(); ++it) {
		if (*it == drawable) {
			children.erase(it);
			delete *it;
			return;
		}
	}
}

void DrawableGroup::clearDrawables() {
	for (auto drawable : children) {
		delete drawable;
	}
	children.clear();
}

DrawableGroup::~DrawableGroup() {
	clearDrawables();
}

std::vector<Drawable*>& DrawableGroup::getChildren() {
	return children;
}
