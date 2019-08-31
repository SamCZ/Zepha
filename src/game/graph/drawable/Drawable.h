//
// Created by aurailus on 09/04/19.
//

#pragma once

#include "../Renderer.h"

class Drawable {
public:
    virtual void update(double delta) {};
    virtual void draw(Renderer& renderer) {};

    virtual bool isVisible() { return visible; }
    virtual void setVisible(bool visible) { this->visible = visible; }

    virtual ~Drawable() = default;
protected:
    bool visible = true;
};

