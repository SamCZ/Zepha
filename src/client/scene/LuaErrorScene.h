//
// Created by aurailus on 2019-12-28.
//

#pragma once

#include "Scene.h"

#include "client/Window.h"
#include "client/gui/Root.h"

namespace Gui { class TextElement; };

class LuaErrorScene : public Scene {
public:
	LuaErrorScene(Client& client, const std::string& err);
	
	void update() override;
	
	void draw() override;
	
private:
	
	Gui::Root root;
};
