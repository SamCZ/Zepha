//
// Created by aurailus on 13/05/19.
//

#pragma once

#include "../../def/item/BlockDef.h"
#include "../../def/ClientGame.h"

class PointedThing {
public:
    enum class Thing {
        ENTITY,
        BLOCK,
        NOTHING
    };

    struct PointedBlock {
        unsigned int blockId;
        glm::ivec3 pos;
        Dir face;
    };

    struct PointedEntity {
        //TODO;
    };

    typedef union {
        PointedBlock block;
        PointedEntity entity;
        unsigned short nothing;
    } PointedThingTarget;

    PointedThing() = default;
    PointedThing(const PointedThing &o) = default;

    Thing thing = PointedThing::Thing::NOTHING;
    PointedThingTarget target = PointedThingTarget{0};
};
