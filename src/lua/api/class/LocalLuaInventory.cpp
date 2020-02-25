//
// Created by aurailus on 2020-02-24.
//

#include "LocalLuaInventory.h"

LocalLuaInventoryList LocalLuaInventory::get_list(std::string name) {
    return LocalLuaInventoryList(*inventory[name]);
}