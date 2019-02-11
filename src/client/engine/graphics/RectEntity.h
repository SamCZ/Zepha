//
// Created by aurailus on 10/02/19.
//

#ifndef ZEUS_RECTENTITY_H
#define ZEUS_RECTENTITY_H


#include "../Entity.h"

class RectEntity : public Entity {
public:
    RectEntity();

    explicit RectEntity(glm::vec4 color);
    RectEntity(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);

    explicit RectEntity(Texture* texture);
private:
    void createColored(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);
    void createTextured(Texture* texture);
};


#endif //ZEUS_RECTENTITY_H
