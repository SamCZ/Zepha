//
// Created by aurailus on 25/12/18.
//

#ifndef ZEUS_TEXTBUILDER_H
#define ZEUS_TEXTBUILDER_H


#include "../engine/graphics/Mesh.h"

class TextBuilder {
public:
    TextBuilder() = default;

    Mesh* build(std::string text);

    ~TextBuilder() = default;
private:
    const static int w = 7;
    const static int h = 9;
};


#endif //ZEUS_TEXTBUILDER_H
