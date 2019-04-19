//
// Created by aurailus on 25/12/18.
//

#ifndef ZEUS_TEXTENTITY_H
#define ZEUS_TEXTENTITY_H


#include "../Entity.h"
#include "TextBuilder.h"

//todo: Un-publicize Entity

class HudText : public Entity {
public:
    HudText() = default;
    explicit HudText(Texture* texture);

    void set(std::string text);
private:
    std::string text;
};


#endif //ZEUS_TEXTENTITY_H
