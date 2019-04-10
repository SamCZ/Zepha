//
// Created by aurailus on 09/04/19.
//

#ifndef ZEUS_DRAWABLEGROUP_H
#define ZEUS_DRAWABLEGROUP_H


#include "Drawable.h"

class DrawableGroup : public Drawable {
public:
    void draw(Renderer& renderer) override;

    void addDrawable(Drawable* drawable);
    void removeDrawable(Drawable* drawable);
    void clearDrawables();

    std::vector<Drawable*>& getChildren();

    ~DrawableGroup() override;
protected:
    std::vector<Drawable*> children {};
};


#endif //ZEUS_DRAWABLEGROUP_H
