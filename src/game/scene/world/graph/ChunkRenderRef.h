//
// Created by aurailus on 29/09/19.
//

#pragma once

#include <list>
#include "ChunkRenderElem.h"
#include "../../../../util/Pointer.h"

struct ChunkRenderRef {
    ChunkRenderRef(std::list<sptr<ChunkRenderElem>>::iterator iter) : iter(iter) {}

    std::list<sptr<ChunkRenderElem>>::iterator iter;
};
