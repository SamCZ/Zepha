////
//// Created by aurailus on 2020-02-19.
////
//
//#include "lua/Lua.h"
//
//#include "LocalModHandler.h"
//
//void LocalModHandler::addLuaMod(const LuaMod& mod) {
//	mods.emplace_back(mod);
//}
//
//void LocalModHandler::setModsOrder(const std::vector<std::string>& order) {
//	modsOrder = order;
//}
//
//void LocalModHandler::executeMods(std::function<void(std::string)> run) {
//	try {
//		for (std::string& modName : modsOrder) {
//			if (modName == "base") {
//				run(modName + "/init");
//				break;
//			}
//		}
//
//		for (std::string& modName : modsOrder) {
//			if (modName != "base") run(modName + "/init");
//		}
//	}
//	catch (sol::error e) {
//		std::cout << "CAUGHT B " << e.what() << std::endl;
//	}
//}
//
//const std::vector<LuaMod>& LocalModHandler::cGetMods() const {
//	return mods;
//}