//
// Created by aurailus on 13/05/19.
//

#ifndef ZEUS_POSBLOCK_H
#define ZEUS_POSBLOCK_H

#include "../../def/block/BlockDef.h"
#include "../../def/GameDefs.h"

struct PosBlock {
    PosBlock() = default;
    PosBlock(const PosBlock &o) = default;

    PosBlock(glm::vec3 pos, unsigned int blockID, GameDefs& defs) :
        blockDef(defs.blocks().getBlock(blockID)),
        blockID(blockID),
        pos(pos) {}

    PosBlock& operator=(const PosBlock &o) {
        this->blockDef = o.blockDef;
        this->blockID = o.blockID;
        this->pos = o.pos;
    }

    bool operator==(const PosBlock &o) {
        return (blockID == o.blockID && pos == o.pos);
    }

    bool operator!=(const PosBlock &o) {
        return (blockID != o.blockID || pos != o.pos);
    }

    BlockDef blockDef;
    unsigned int blockID = 0;
    glm::vec3 pos {};
};

#endif //ZEUS_POSBLOCK_H
