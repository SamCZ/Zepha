template <class T>
void RegionHandler<T>::addChunk(glm::vec3 pos, T *chunk) {

    glm::vec3 regionPos = TransPos::Dimension::regionFromVec(pos);
    if (!regions.count(regionPos)) regions.insert({regionPos, new Region<T>(regionPos)});
    Region<T>* region = regions[regionPos];

    glm::vec3 mapBlockPos = TransPos::Dimension::mapBlockOffsetFromRegion(pos);
    unsigned int mapBlockInd = TransPos::mapBlockIndFromVec(mapBlockPos);
    if ((*region)[mapBlockInd] == nullptr) region->set(mapBlockInd, new MapBlock<T>(mapBlockPos));
    MapBlock<T>* mapBlock = (*region)[mapBlockInd];

    glm::vec3 chunkPos = TransPos::Dimension::chunkOffsetFromMapBlock(pos);
    unsigned int chunkInd = TransPos::chunkIndFromVec(chunkPos);
    delete (*mapBlock)[chunkInd];
    mapBlock->set(chunkInd, chunk);
}

template <class T>
T *RegionHandler<T>::getChunk(glm::vec3 pos) {

    glm::vec3 regionPos = TransPos::Dimension::regionFromVec(pos);
    if (!regions.count(regionPos)) return nullptr;
    Region<T>* region = regions[regionPos];

    glm::vec3 mapBlockPos = TransPos::Dimension::mapBlockOffsetFromRegion(pos);
    unsigned int mapBlockInd = TransPos::mapBlockIndFromVec(mapBlockPos);
    if ((*region)[mapBlockInd] == nullptr) return nullptr;
    MapBlock<T>* mapBlock = (*region)[mapBlockInd];

    glm::vec3 chunkPos = TransPos::Dimension::chunkOffsetFromMapBlock(pos);
    unsigned int chunkInd = TransPos::chunkIndFromVec(chunkPos);
    return (*mapBlock)[chunkInd];
}

template <class T>
Region<T>* RegionHandler<T>::getRegion(glm::vec3 pos) {
    return regions[pos];
}

template <class T>
RegionHandler<T>::~RegionHandler() {
    for (std::pair<glm::vec3, Region<T>*> region : regions) {
        delete region.second;
    }
}
