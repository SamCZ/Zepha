//
// Created by aurailus on 16/04/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma clang diagnostic pop

#include "TextureAtlas.h"

#include <cute_files.h>
#include <stb_image.h>
#include <stb_image_write.h>

//Height is optional and defaults to 0
TextureAtlas::TextureAtlas(unsigned int width, unsigned int height) :
    pageWidth(width),
    pageHeight((height == 0 ? width : height)),
    pageTileWidth(pageWidth / 16),
    pageTileHeight(pageHeight / 16),
    atlasData(new unsigned char[pageWidth * 4 * pageHeight]) {

    int maxTexSize, texUnits;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << "This GPU's max texture size is: " << maxTexSize / 4 << "px^2." << std::endl;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
    std::cout << "This GPU supports " << texUnits << " texture units." << std::endl;

    empty = std::vector<bool>(pageTileWidth * pageTileHeight, true);
    for (int i = 0; i < pageWidth * 4 * pageHeight; i++) atlasData[i] = 0;

    t = new Texture();
    t->loadFromBytes(atlasData, pageWidth, pageHeight);

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
            delete[] data;
        }

        cf_dir_next(&dir);
    }
    cf_dir_close(&dir);

//    std::string trypls("default_grass_top");
//    generateCrackImage(trypls);
//    trypls = "default_sand";
//    generateCrackImage(trypls);
//    trypls = "default_log_side";
//    generateCrackImage(trypls);
//    trypls = "default_leaves";
//    generateCrackImage(trypls);
//    trypls = "default_grass_float";
//    generateCrackImage(trypls);
//    trypls = "_missing";
//    generateCrackImage(trypls);
//    trypls = "default_leaves_puff";
//    generateCrackImage(trypls);
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
            std::cerr << "Failed to find space in dynamic atlas." << std::endl;
            return nullptr;
        }

        ref->tileX = static_cast<int>(space.x);
        ref->tileY = static_cast<int>(space.y);

        ref->uv = {space.x / pageTileWidth, space.y / pageTileHeight,
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

    t->updateTexture(baseX, baseY, texWidth, texHeight, data);

    for (int i = 0; i < texWidth * texHeight * 4; i++) {
        int xx = (i / 4) % texWidth;
        int yy = (i / 4) / texWidth;
        int of = i % 4;

        atlasData[(baseX + xx + (baseY + yy) * pageWidth) * 4 + of] = data[(xx + yy * texWidth) * 4 + of];
    }
}

TextureAtlas::RawTexData TextureAtlas::getSubImageBytes(std::string &name) {
    glm::vec4 uvs;

    if (textures.count(name)) {
        uvs = textures[name]->uv;
    }
    else {
        std::cerr << "Invalid base texture " << name << "." << std::endl;
        uvs = textures["_missing"]->uv;
    }

    RawTexData data {};
    data.width = static_cast<int>((uvs.z - uvs.x) * pageWidth);
    data.height = static_cast<int>((uvs.w - uvs.y) * pageHeight);

    auto pixels = new unsigned char[data.width * data.height * 4];

    int x = static_cast<int>(uvs.x * pageWidth);
    int y = static_cast<int>(uvs.y * pageWidth);

    for (int i = 0; i < data.width * data.height; i++) {
        int xx = x + (i % data.width);
        int yy = y + (i / data.width);

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
        float alpha = crack.data[i * 4 + 3] / 255.f * (crackLevel / 16.f + 0.5f);

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
    return *t;
}

std::shared_ptr<AtlasRef> TextureAtlas::getTextureRef(std::string &name) {
    if (!textures.count(name)) {
//        return tryMakeGraphics(name);
    }
    return textures[name];
}

TextureAtlas::~TextureAtlas() = default;