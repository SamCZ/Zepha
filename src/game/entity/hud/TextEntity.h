//
// Created by aurailus on 25/12/18.
//

#ifndef ZEUS_TEXTENTITY_H
#define ZEUS_TEXTENTITY_H


#include "../Entity.h"

class TextEntity : public Entity {
public:
    TextEntity() = default;
    explicit TextEntity(Texture* texture, bool background = false, int scale = 2);

    void set(std::string text);
    std::string get();
private:
    int getWidthSubtract(int c);

    const static int w = 7;
    const static int h = 9;

    bool background = false;
    std::string text;
};


#endif //ZEUS_TEXTENTITY_H
