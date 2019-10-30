//
// Created by aurailus on 13/05/19.
//

#pragma once

#include "../../def/item/BlockDef.h"
#include "../../def/LocalDefs.h"

class PointedThing {
public:
    enum class Thing {
        ENTITY,
        BLOCK,
        NOTHING
    };

    struct PointedBlock {
        unsigned int blockId;
        glm::vec3 pos;
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

//    PointedThing& operator=(const PointedThing &o) {
//        this->blockDef = o.blockDef;
//        this->blockID = o.blockID;
//        this->pos = o.pos;
//    }
//
//    bool operator==(const PointedThing &o) {
//        return (blockID == o.blockID && pos == o.pos);
//    }
//
//    bool operator!=(const PointedThing &o) {
//        return (blockID != o.blockID || pos != o.pos);
//    }

    Thing thing = PointedThing::Thing::NOTHING;
    PointedThingTarget target = PointedThingTarget{0};
};
