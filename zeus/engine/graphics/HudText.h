//
// Created by aurailus on 25/12/18.
//

#ifndef ZEUS_TEXTENTITY_H
#define ZEUS_TEXTENTITY_H


#include "../Entity.h"

class HudText : public Entity {
public:
    HudText();
    HudText(Texture* texture);

    void set(std::string text);
private:
    std::string text;
};


#endif //ZEUS_TEXTENTITY_H
