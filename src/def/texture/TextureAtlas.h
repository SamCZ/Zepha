//
// Created by aurailus on 16/04/19.
//

#ifndef ZEUS_DYNAMICATLAS_H
#define ZEUS_DYNAMICATLAS_H

#include <vector>
#include <vec2.hpp>
#include <glew.h>
#include <tgmath.h>
#include <memory>
#include <map>
#include "../../game/graph/Texture.h"
#include "AtlasRef.h"

class TextureAtlas {
public:
    TextureAtlas() = default;
    explicit TextureAtlas(unsigned int width, unsigned int height = 0);

    void loadDirectory(std::string dirStr);

    Texture& getAtlasTexture();

    std::shared_ptr<AtlasRef> addImage(unsigned char *data, std::string name, int texWidth, int texHeight);

    glm::vec4 getTextureUVs(std::string &name);

    ~TextureAtlas();
private:
    struct RawTexData {
        unsigned char* data;
        int width;
        int height;
    };

    RawTexData getSubImageBytes(std::string &name);
    glm::vec4 tryMakeGraphics(std::string &name);

    glm::vec2 findImageSpace(int w, int h);

    void createMissingImage();
    void updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data);

    unsigned int pageWidth = 0;
    unsigned int pageHeight = 0;
    unsigned int pageTileWidth = 0;
    unsigned int pageTileHeight = 0;

    Texture* t = nullptr;
    unsigned char* atlasData = nullptr;

    std::vector<bool> empty;
    std::map<std::string, std::shared_ptr<AtlasRef>> textures;
};


#endif //ZEUS_DYNAMICATLAS_H
