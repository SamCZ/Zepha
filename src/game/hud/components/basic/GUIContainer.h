//
// Created by aurailus on 27/07/19.
//

#pragma once


#include "../GUIComponent.h"

class GUIContainer : public GUIComponent {
public:
    GUIContainer() = default;
    explicit GUIContainer(const std::string& key);

    void draw(Renderer& renderer) override;
};

