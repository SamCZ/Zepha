////
//// Created by aurailus on 2019-10-29.
////
//
//#include "GuiInventoryItem.h"
//
//#include "GuiText.h"
//#include "GuiRect.h"
//#include "GuiModel.h"
//#include "client/graph/Font.h"
//#include "game/def/BlockDef.h"
//#include "game/def/CraftItemDef.h"
//
//void GuiInventoryItem::create(glm::vec2 scale, unsigned short count, ItemDef& def, Font f) {
//
//	if (def.type == ItemDef::Type::CRAFTITEM) {
//		auto texture = static_cast<CraftItemDef&>(def).textureRefs[0];
//
//		auto shadow = std::make_shared<GuiRect>(window, "shadow");
//		shadow->create(scale * 16.f, {}, texture, { 0, 0, 0, 0.2 });
//		add(shadow);
//		shadow->setPos(scale);
//
//		auto item = std::make_shared<GuiRect>(window, "mesh");
//		item->create(scale * 16.f, {}, texture);
//		add(item);
//	}
//	else {
//		auto& model = static_cast<BlockDef&>(def).entityModel;
//
//		auto item = std::make_shared<GuiModel>(window, "mesh");
//		item->create(scale * 10.5f, model);
//		item->setPos(glm::vec2{ 8, 8 } * scale);
//
//		item->setRotationX(180.f - 30.f);
//		item->setRotationY(45.f);
//		item->setRotationZ(0.f);
//
//		add(item);
//	}
//
//	if (count > 1) {
//		auto text = std::make_shared<GuiText>(window, "count");
//		text->create(scale, {}, { 1, 1, 1, 1 }, {}, f);
//		text->setText(std::to_string(count));
//		add(text);
//		text->setPos({ (19 - text->getWidth()) * scale.x, 9 * scale.y });
//	}
//}
