//
// Created by aurailus on 11/08/19.
//

#ifndef ZEUS_ITEMDEF_H
#define ZEUS_ITEMDEF_H

#include <string>

class ItemDef {
public:
    enum class Type {
        INVALID,
        BLOCK,
        CRAFTITEM,
        TOOL
    };

    std::string identifier = "";
    unsigned int index = 0;

    Type type = Type::INVALID;
};


#endif //ZEUS_ITEMDEF_H
