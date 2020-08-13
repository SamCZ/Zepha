//
// Created by aurailus on 16/04/19.
//

#pragma once

#include <map>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "client/graph/Texture.h"

class AtlasRef;

class TextureAtlas {
public:
    struct RawTexData {
        unsigned char* data;
        int width;
        int height;
    };

    TextureAtlas() = default;
    explicit TextureAtlas(unsigned int width, unsigned int height = 0);

    void update();

    std::vector<std::shared_ptr<AtlasRef>> loadDirectory(const std::string& path, bool base = true, bool recursive = true);
    std::shared_ptr<AtlasRef> loadImage(const std::string& path, const std::string& name, bool base = false);

    glm::vec4 sampleTexturePixel(const std::shared_ptr<AtlasRef>& atlasRef, glm::vec2 pixel);
    std::shared_ptr<AtlasRef> operator[](const std::string& name);

    std::shared_ptr<AtlasRef> addImage(unsigned char *data, const std::string& name, bool base, int texWidth, int texHeight);
    std::shared_ptr<AtlasRef> generateCrackImage(const std::string &name, unsigned short crackLevel);

    glm::ivec2 pixelSize {};
    glm::ivec2 tileSize {};

    Texture atlasTexture {};
    std::vector<unsigned char> atlasData;

    unsigned int textureSlotsUsed = 0;
    unsigned int maxTextureSlots = 0;
private:
    std::shared_ptr<AtlasRef> generateTexture(std::string req);

    RawTexData getBytesOfTex(const std::string& name);
    RawTexData getBytesAtPos(glm::ivec2 pos, glm::ivec2 dims);
    glm::vec2 findImageSpace(int w, int h);

    void createMissingImage();
    void updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data);

    void deleteImage(std::shared_ptr<AtlasRef> ref);

    std::map<std::string, std::shared_ptr<AtlasRef>> textures;
    std::vector<bool> empty;
};

