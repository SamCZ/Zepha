//
// Created by aurailus on 09/04/19.
//

#ifndef ZEUS_DRAWABLE_H
#define ZEUS_DRAWABLE_H


#include "../Renderer.h"

class Drawable {
public:
    virtual void draw(Renderer& renderer) {};

    virtual ~Drawable() = default;

    bool isVisible() {
        return visible;
    }

    void setVisible(bool visible) {
        this->visible = visible;
    }
protected:
    bool visible = true;
};


#endif //ZEUS_DRAWABLE_H
