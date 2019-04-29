//
// Created by aurailus on 10/02/19.
//

#ifndef ZEUS_RECTENTITY_H
#define ZEUS_RECTENTITY_H


#include "../Entity.h"

class TextureRect : public Entity {
public:
    TextureRect() = default;

    explicit TextureRect(glm::vec4 color);
    TextureRect(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);

    void setTexture(Texture* texture);

    explicit TextureRect(Texture* texture);
private:
    void createColored(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);
    void createTextured(Texture* texture);

    using Entity::setTexture;
};


#endif //ZEUS_RECTENTITY_H
