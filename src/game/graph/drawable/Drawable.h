//
// Created by aurailus on 09/04/19.
//

#pragma once


#include "../Renderer.h"

class Drawable {
public:
    virtual void draw(Renderer& renderer) {};

    virtual ~Drawable() = default;

    virtual bool isVisible() {
        return visible;
    }

    virtual void setVisible(bool visible) {
        this->visible = visible;
    }
protected:
    bool visible = true;
};

