//
// Created by aurailus on 13/05/19.
//

#ifndef ZEUS_POSBLOCK_H
#define ZEUS_POSBLOCK_H

#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/LocalDefs.h"

struct PointedThing {
    PointedThing() = default;
    PointedThing(const PointedThing &o) = default;

    PointedThing& operator=(const PointedThing &o) {
        this->blockDef = o.blockDef;
        this->blockID = o.blockID;
        this->pos = o.pos;
    }

    bool operator==(const PointedThing &o) {
        return (blockID == o.blockID && pos == o.pos);
    }

    bool operator!=(const PointedThing &o) {
        return (blockID != o.blockID || pos != o.pos);
    }

public:
    //Properties
    unsigned int blockID = 0;
    LocalBlockDef* blockDef = nullptr;

    glm::vec3 pos {};
    Dir face = NONE;
};

#endif //ZEUS_POSBLOCK_H
