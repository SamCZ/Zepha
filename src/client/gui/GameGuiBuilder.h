////
//// Created by aurailus on 2019-12-12.
////
//
//#pragma once
//
//#include "GuiBuilder.h"
//
//#include "world/inv/ServerInventory.h"
//#include "world/inv/LocalInventoryRefs.h"
//
//class GameGuiBuilder : public GuiBuilder {
//	public:
//	GameGuiBuilder(InventoryRefsPtr refs, Window& window, SubgamePtr defs, std::shared_ptr<GuiContainer> root) :
//		defs(defs), refs(refs), GuiBuilder(defs.l()->textures, window, defs.l()->models, root) {};
//
//	std::shared_ptr<Element> createComponent(LuaGuiElement& elem, glm::ivec2 bounds) override;
//
//	private:
//	InventoryRefsPtr refs;
//	SubgamePtr defs;
//};
