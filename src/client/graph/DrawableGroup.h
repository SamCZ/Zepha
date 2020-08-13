//
// Created by aurailus on 09/04/19.
//

#pragma once

#include <vector>

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

