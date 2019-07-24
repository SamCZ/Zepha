//
// Created by aurailus on 16/04/19.
//

#include "TextureAtlas.h"
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>

//Height is optional and defaults to 0
TextureAtlas::TextureAtlas(unsigned int width, unsigned int height) :
    pageWidth(width),
    pageHeight((height == 0 ? width : height)),
    pageTileWidth(pageWidth / 16),
    pageTileHeight(pageHeight / 16),
    atlasData(new unsigned char[pageWidth * 4 * pageHeight]) {

    int maxTexSize, texUnits;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << Log::info << "This GPU's max texture size is: " << maxTexSize / 4 << "px^2." << Log::endl;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
    std::cout << Log::info << "This GPU supports " << texUnits << " texture units." << Log::endl;

    empty = std::vector<bool>(pageTileWidth * pageTileHeight, true);
    for (int i = 0; i < pageWidth * 4 * pageHeight; i++) atlasData[i] = 0;

    t.loadFromBytes(atlasData, pageWidth, pageHeight);

    createMissingImage();
}

void TextureAtlas::createMissingImage() {
    auto data = new unsigned char[16 * 4 * 16];
    for (int i = 0; i < 16 * 16; i++) {

        unsigned char m = 0;
        if ((i % 16 < 8) ^ ((i / 16) < 8)) m = 255;

        data[i * 4 + 0] = m;
        data[i * 4 + 1] = 0;
        data[i * 4 + 2] = m;
        data[i * 4 + 3] = 255;
    }

    addImage(data, "_missing", true, 16, 16);
    delete [] data;
}

void TextureAtlas::loadDirectory(std::string dirStr) {
    cf_dir_t dir;
    cf_dir_open(&dir, (dirStr).c_str());

    while (dir.has_next) {
        cf_file_t file;
        cf_read_file(&dir, &file);

        if (!file.is_dir && strcmp(file.ext, ".png") == 0) {
            int width, height;
            unsigned char* data = stbi_load(file.path, &width, &height, nullptr, 4);
            addImage(data, std::string(file.name).substr(0, std::string(file.name).size() - 4), true, width, height);
            delete data;
        }

        cf_dir_next(&dir);
    }
    cf_dir_close(&dir);
}

void TextureAtlas::update() {
    auto it = textures.cbegin();

    while (it != textures.cend()) {
        auto curr = it++;

        if (!curr->second->base && curr->second.unique()) {
            deleteImage(curr->second);
            textures.erase(curr);
        }
    }
}

glm::vec2 TextureAtlas::findImageSpace(int w, int h) {
    for (int j = 0; j < pageTileHeight - (h - 1); j++) {
        for (int i = 0; i < pageTileWidth - (w - 1); i++) {
            if (empty[j * pageTileWidth + i]) {
                bool space = true;

                for (int k = 0; k < h; k++) {
                    for (int l = 0; l < w; l++) {
                        if (!empty[(j + k) * pageTileWidth + (i + l)]) {
                            space = false;
                            break;
                        }
                    }

                    if (!space) break;
                }

                if (space) {
                    for (int k = 0; k < h; k++) {
                        for (int l = 0; l < w; l++) {
                            empty[(j + k) * pageTileWidth + (i + l)] = false;
                        }
                    }
                    return glm::vec2(i, j);
                }
            }
        }
    }

    return glm::vec2(-1, -1);
}

std::shared_ptr<AtlasRef> TextureAtlas::addImage(unsigned char *data, std::string name, bool base, int texWidth, int texHeight) {
    std::shared_ptr<AtlasRef> ref;

    if (textures.count(name) != 0) ref = textures[name];
    else ref = std::make_shared<AtlasRef>();

    ref->name = name;
    ref->base = base;
    ref->width = texWidth;
    ref->height = texHeight;

    auto tileWidth = static_cast<int>(std::ceil(texWidth / 16.0f));
    auto tileHeight = static_cast<int>(std::ceil(texHeight / 16.0f));

    if (tileWidth != ref->tileWidth || tileHeight != ref->tileHeight) {
        ref->tileWidth = tileWidth;
        ref->tileHeight = tileHeight;

        auto space = findImageSpace(tileWidth, tileHeight);

        if (space.x < 0) {
            std::cout << Log::err << "Failed to find space in dynamic atlas." << Log::endl;
            return nullptr;
        }

        ref->tileX = static_cast<int>(space.x);
        ref->tileY = static_cast<int>(space.y);

        ref->pos = {space.x * 16, space.y * 16, space.x * 16 + texWidth, space.y * 16 + texHeight};
        ref->uv = {(space.x * 16) / pageWidth, (space.y * 16) / pageHeight,
                   (space.x * 16 + texWidth) / pageWidth, (space.y * 16 + texHeight) / pageHeight};

        textures.insert({name, ref});
    }

    updateAtlas(ref->tileX, ref->tileY, texWidth, texHeight, data);
    return ref;
}

void TextureAtlas::deleteImage(std::shared_ptr<AtlasRef> ref) {
    //For Debugging
//    auto data = new unsigned char[ref->width * ref->height * 4];
//
//    for (int i = 0; i < ref->width * ref->height * 4; i++) {
//        data[i] = 0;
//    }
//
//    updateAtlas(ref->tileX, ref->tileY, ref->width, ref->height, data);
//    delete[] data;

    for (auto i = ref->tileX; i < ref->tileX + ref->tileWidth; i++) {
        for (auto j = ref->tileY; j < ref->tileY + ref->tileHeight; j++) {
            empty[j * pageTileWidth + i] = true;
        }
    }
}

void TextureAtlas::updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data) {
    int baseX = tileX * 16;
    int baseY = tileY * 16;

    t.updateTexture(baseX, baseY, texWidth, texHeight, data);

    for (int i = 0; i < texWidth * texHeight * 4; i++) {
        int xx = (i / 4) % texWidth;
        int yy = (i / 4) / texWidth;
        int of = i % 4;

        atlasData[(baseX + xx + (baseY + yy) * pageWidth) * 4 + of] = data[(xx + yy * texWidth) * 4 + of];
    }
}

TextureAtlas::RawTexData TextureAtlas::getSubImageBytes(std::string &name) {
    glm::vec4 pos;

    if (textures.count(name)) {
        pos = textures[name]->pos;
    }
    else {
        std::cout << Log::err << "Invalid base texture \"" << name << "\"." << Log::endl;
        pos = textures["_missing"]->pos;
    }

    RawTexData data {};
    data.width = static_cast<int>(pos.z - pos.x);
    data.height = static_cast<int>(pos.w - pos.y);

    auto pixels = new unsigned char[data.width * data.height * 4];

    for (int i = 0; i < data.width * data.height; i++) {
        int xx = static_cast<int>(pos.x) + (i % data.width);
        int yy = static_cast<int>(pos.y) + (i / data.width);

        pixels[i * 4 + 0] = atlasData[xx * 4     + yy * (pageWidth * 4)];
        pixels[i * 4 + 1] = atlasData[xx * 4 + 1 + yy * (pageWidth * 4)];
        pixels[i * 4 + 2] = atlasData[xx * 4 + 2 + yy * (pageWidth * 4)];
        pixels[i * 4 + 3] = atlasData[xx * 4 + 3 + yy * (pageWidth * 4)];
    }

    data.data = pixels;

    return data;
}

std::shared_ptr<AtlasRef> TextureAtlas::generateCrackImage(std::string &name, unsigned short crackLevel) {
    RawTexData base = getSubImageBytes(name);

    std::string crackStr("default_crack_" + std::to_string(crackLevel));
    RawTexData crack = getSubImageBytes(crackStr);

    for (int i = 0; i < base.width * base.height; i++) {
//        float alpha = crack.data[i * 4 + 3] / 255.f * (crackLevel / 16.f + 0.5f);
        float alpha = crack.data[i * 4 + 3] / 255.f;
        
        base.data[i * 4 + 0] = static_cast<unsigned char>(base.data[i * 4 + 0] * (1 - alpha) + crack.data[i * 4 + 0] * alpha);
        base.data[i * 4 + 1] = static_cast<unsigned char>(base.data[i * 4 + 1] * (1 - alpha) + crack.data[i * 4 + 1] * alpha);
        base.data[i * 4 + 2] = static_cast<unsigned char>(base.data[i * 4 + 2] * (1 - alpha) + crack.data[i * 4 + 2] * alpha);
    }

    auto ref = addImage(base.data, name + "_crack_" + std::to_string(crackLevel), false, base.width, base.height);

    delete [] base.data;
    delete [] crack.data;

    return ref;
}

Texture &TextureAtlas::getAtlasTexture() {
    return t;
}

std::shared_ptr<AtlasRef> TextureAtlas::getTextureRef(const std::string &name) {
    if (!textures.count(name)) {
        std::cout << Log::err << "Invalid texture name: \"" << name << "\"." << Log::endl;
//        return tryMakeGraphics(name);
    }
    return textures[name];
}

TextureAtlas::~TextureAtlas() = default;
//    delete[] atlasData;
//    delete t;