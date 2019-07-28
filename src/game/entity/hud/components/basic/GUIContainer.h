//
// Created by aurailus on 27/07/19.
//

#ifndef ZEUS_GUICONTAINER_H
#define ZEUS_GUICONTAINER_H


#include "../GUIComponent.h"

class GUIContainer : public GUIComponent {
public:
    GUIContainer() = default;
    explicit GUIContainer(const std::string& key);

    void draw(Renderer& renderer) override;
};


#endif //ZEUS_GUICONTAINER_H
