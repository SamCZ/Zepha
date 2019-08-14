//
// Created by aurailus on 16/04/19.
//

#ifndef ZEUS_DYNAMICATLAS_H
#define ZEUS_DYNAMICATLAS_H

#include <map>
#include <memory>
#include <vector>
#include <tgmath.h>
#include <glm/vec2.hpp>
#include <glew/glew.h>
#include <cute_files/cute_files.h>
#include "../../util/Log.h"
#include "../../game/graph/Texture.h"
#include "AtlasRef.h"

class TextureAtlas {
public:
    TextureAtlas() = default;
    explicit TextureAtlas(unsigned int width, unsigned int height = 0);

    void loadDirectory(std::string dirStr);
    void update();

    Texture& getAtlasTexture();
    const unsigned char* getAtlasData();
    glm::vec2 getAtlasSize();

    std::shared_ptr<AtlasRef> addImage(unsigned char *data, std::string name, bool base, int texWidth, int texHeight);
    std::shared_ptr<AtlasRef> generateCrackImage(std::string &name, unsigned short crackLevel);

    std::shared_ptr<AtlasRef> getTextureRef(const std::string &name);

    ~TextureAtlas();
private:
    struct RawTexData {
        unsigned char* data;
        int width;
        int height;
    };

    RawTexData getSubImageBytes(std::string &name);

    glm::vec2 findImageSpace(int w, int h);

    void createMissingImage();
    void updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data);

    void deleteImage(std::shared_ptr<AtlasRef> ref);

    unsigned int pageWidth = 0;
    unsigned int pageHeight = 0;
    unsigned int pageTileWidth = 0;
    unsigned int pageTileHeight = 0;

    Texture t;
    unsigned char* atlasData = nullptr;

    std::vector<bool> empty;
    std::map<std::string, std::shared_ptr<AtlasRef>> textures;
};


#endif //ZEUS_DYNAMICATLAS_H
