glm::ivec3 Chunk::getPos() const {
    auto l = getReadLock();
    return pos;
}

void Chunk::setPos(glm::ivec3 pos) {
    auto l = getWriteLock();
    this->pos = pos;
}

bool Chunk::isDirty() const {
    auto l = getReadLock();
    return dirty;
}

void Chunk::setDirty(bool dirty) {
    auto l = getWriteLock();
    this->dirty = dirty;
}

bool Chunk::chunkShouldRender() const {
    auto l = getReadLock();
    return shouldRender;
}

void Chunk::setShouldRender(bool shouldRender) {
    auto l = getWriteLock();
    this->shouldRender = shouldRender;
}

bool Chunk::isPartial() const {
    auto l = getReadLock();
    return partial;
}

void Chunk::setPartial(bool partial) {
    auto l = getWriteLock();
    this->partial = partial;
};

bool Chunk::isGenerated() const {
    auto l = getReadLock();
    return generated;
}

void Chunk::setGenerated(bool generated) {
    auto l = getWriteLock();
    this->generated = generated;
}

inline unsigned int Chunk::getBlock(unsigned int ind) const {
    auto l = getReadLock();
    if (ind >= 4096) return 0; // Invalid
    return RIE::read<unsigned int>(ind, blocks, 4096);
}

inline unsigned int Chunk::getBlock(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return 0; // Invalid
    return getBlock(Space::Block::index(pos));
}

inline bool Chunk::setBlock(const glm::ivec3& pos, unsigned int blk) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBlock(Space::Block::index(pos), blk);
}

inline unsigned short Chunk::getBiome(unsigned int ind) const {
    auto l = getReadLock();
    if (ind >= 4096) return 0; // Invalid
    return RIE::read<unsigned short>(ind, biomes, 4096);
}

inline unsigned short Chunk::getBiome(const glm::ivec3& pos) const {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return 0; // Invalid
    return getBiome(Space::Block::index(pos));
}

inline bool Chunk::setBiome(unsigned int ind, unsigned short bio) {
    auto l = getWriteLock();
    return RIE::write(ind, bio, biomes, 4096);
}

inline bool Chunk::setBiome(const glm::ivec3& pos, unsigned short bio) {
    if (pos.x > 15 || pos.x < 0 || pos.y > 15 || pos.y < 0 || pos.z > 15 || pos.z < 0) return false;
    return setBiome(Space::Block::index(pos), bio);
}

inline glm::ivec4 Chunk::getLight(unsigned int ind) {
    auto l = getReadLock();
    return { blockLight[ind].r, blockLight[ind].g, blockLight[ind].b, getSunlight(ind) };
}

inline void Chunk::setLight(unsigned int ind, glm::ivec4 light) {
    auto l = getWriteLock();
    blockLight[ind].r = light.x;
    blockLight[ind].g = light.y;
    blockLight[ind].b = light.z;
    l.unlock();
    setSunlight(ind, light.w);
}

inline unsigned char Chunk::getLight(unsigned int ind, unsigned char channel) {
    auto l = getReadLock();
    return channel == 0 ? blockLight[ind].r :
           channel == 1 ? blockLight[ind].g :
           channel == 2 ? blockLight[ind].b :
           (l.unlock(), getSunlight(ind));
}

inline void Chunk::setLight(unsigned int ind, unsigned char channel, unsigned char light) {
    auto l = getWriteLock();
    channel == 0 ? blockLight[ind].r = light :
    channel == 1 ? blockLight[ind].g = light :
    channel == 2 ? blockLight[ind].b = light :
    (l.unlock(), setSunlight(ind,light), 0);
}

inline unsigned char Chunk::getSunlight(unsigned int ind) {
    auto l = getReadLock();
    if (ind % 2 == 0) return sunLight[ind / 2].a;
    else return sunLight[ind / 2].b;
}

inline void Chunk::setSunlight(unsigned int ind, unsigned char val) {
    auto l = getWriteLock();
    if (ind % 2 == 0) sunLight[ind / 2].a = val;
    else sunLight[ind / 2].b = val;
}