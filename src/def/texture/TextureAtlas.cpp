//
// Created by aurailus on 16/04/19.
//

#include <cmath>
#include <iostream>
#include <algorithm>
#include <stb_image/stb_image.h>
#include <cute_files/cute_files.h>

#include "TextureAtlas.h"

#include "AtlasRef.h"
#include "../../util/Log.h"

TextureAtlas::TextureAtlas(unsigned int width, unsigned int height) :
    pixelSize(width, (height == 0 ? width : height)),
    tileSize(pixelSize.x / 16, pixelSize.y / 16),
    atlasData(pixelSize.x * 4 * pixelSize.y) {

    maxTextureSlots = tileSize.x * tileSize.y;

//    int maxTexSize, texUnits;
//
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
//    std::cout << Log::info << "This GPU's max texture size is: " << maxTexSize / 4 << "px^2." << Log::endl;
//
//    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
//    std::cout << Log::info << "This GPU supports " << texUnits << " texture units." << Log::endl;

    empty = std::vector<bool>(tileSize.x * tileSize.y, true);
    atlasTexture.loadFromBytes(&atlasData[0], pixelSize.x, pixelSize.y);

    createMissingImage();
}

std::vector<std::shared_ptr<AtlasRef>> TextureAtlas::loadDirectory(const std::string& path, bool base, bool recursive) {
    std::vector<std::shared_ptr<AtlasRef>> refs {};

    cf_dir_t dir;
    cf_dir_open(&dir, (path).c_str());

    while (dir.has_next) {
        cf_file_t file;
        cf_read_file(&dir, &file);

        if (!file.is_dir && strcmp(file.ext, ".png") == 0) {
            refs.push_back(loadImage(file.path, std::string(file.name).substr(0, std::string(file.name).size() - 4), base));
        }

        if (recursive && file.is_dir && strncmp(file.name, ".", 1) != 0) {
            auto vec = loadDirectory(file.path, base, recursive);
            refs.insert(refs.end(), vec.begin(), vec.end());
        }

        cf_dir_next(&dir);
    }
    cf_dir_close(&dir);

    return refs;
}

std::shared_ptr<AtlasRef> TextureAtlas::loadImage(const std::string &path, const std::string &name, bool base) {
    int width, height;
    unsigned char* data = stbi_load(path.data(), &width, &height, nullptr, 4);
    auto ref = addImage(data, name, base, width, height);
    free(data);
    return ref;
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

glm::vec4 TextureAtlas::sampleTexturePixel(const std::shared_ptr<AtlasRef> &atlasRef, glm::vec2 pixel) {
    glm::vec2 absPos = {atlasRef->pos.x + pixel.x, atlasRef->pos.y + pixel.y};
    unsigned int index = (static_cast<unsigned int>(absPos.y) * pixelSize.x + static_cast<unsigned int>(absPos.x)) * 4;

    return {
        static_cast<float>(atlasData[index]) / 255.f,
        static_cast<float>(atlasData[index + 1]) / 255.f,
        static_cast<float>(atlasData[index + 2]) / 255.f,
        static_cast<float>(atlasData[index + 3]) / 255.f,
    };
}

std::shared_ptr<AtlasRef> TextureAtlas::addImage(unsigned char *data, const std::string& name, bool base, int texWidth, int texHeight) {
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
        if (space.x < 0) throw std::runtime_error("Failed to find space in the dynamic definition atlas.");

        textureSlotsUsed += tileWidth * tileHeight;

        ref->tileX = static_cast<int>(space.x);
        ref->tileY = static_cast<int>(space.y);

        ref->pos = {space.x * 16, space.y * 16, space.x * 16 + texWidth, space.y * 16 + texHeight};
        ref->uv = {(space.x * 16) / pixelSize.x, (space.y * 16) / pixelSize.y,
                   (space.x * 16 + texWidth) / pixelSize.x, (space.y * 16 + texHeight) / pixelSize.y};

        textures.insert({name, ref});
    }

    updateAtlas(ref->tileX, ref->tileY, texWidth, texHeight, data);
    return ref;
}

std::shared_ptr<AtlasRef> TextureAtlas::generateCrackImage(const std::string& name, unsigned short crackLevel) {
    RawTexData base = getBytesOfTex(name);

    std::string crackStr("zeus:default:crack_" + std::to_string(crackLevel));
    RawTexData crack = getBytesOfTex(crackStr);

    for (int i = 0; i < base.width * base.height; i++) {
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

std::shared_ptr<AtlasRef> TextureAtlas::operator[](const std::string &name) {
    if (textures.count(name)) return textures[name];

    std::shared_ptr<AtlasRef> gen = generateTexture(name);
    if (gen) return gen;

    throw std::runtime_error("Invalid texture name " + name + ".");
    return textures["_missing"];
}

std::shared_ptr<AtlasRef> TextureAtlas::generateTexture(std::string req) {
    req.erase(std::remove(req.begin(), req.end(), ' '), req.end());

    if (req.find_first_of('(') != std::string::npos) {
        if (req.find_last_of(')') == std::string::npos) {
            throw "Miasmatched braces.";
        }

        std::string::size_type paramsBegin = req.find_first_of('(');
        std::string::size_type paramsEnd = req.find_last_of(')');

        std::string paramName = req.substr(0, paramsBegin);
        std::string paramsString = req.substr(paramsBegin + 1, paramsEnd - paramsBegin - 1);

        std::vector<std::string> params;
        std::string::size_type pos = 0;
        while ((pos = paramsString.find(',')) != std::string::npos) {
            params.push_back(paramsString.substr(0, pos));
            paramsString.erase(0, pos + 1);
        }
        params.push_back(paramsString);

        if (paramName == "crop") {
            if (params.size() != 5) throw std::runtime_error("crop() requires 5 parameters.");
            glm::ivec4 loc = {atof(params[0].data()), atof(params[1].data()), atof(params[2].data()), atof(params[3].data())};
            std::shared_ptr<AtlasRef> src = operator[](params[4]);

            auto data = getBytesAtPos({src->pos.x + loc.x, src->pos.y + loc.y}, {loc.z, loc.w}).data;
            return addImage(data, req, false, loc.z, loc.w);
        }
        else {
            throw std::runtime_error("Invalid parameter.");
            return nullptr;
        }
    }

    return nullptr;
}

RawTexData TextureAtlas::getBytesOfTex(const std::string &name) {
    glm::vec4 pos;

    if (textures.count(name)) pos = textures[name]->pos;
    else {
        std::cout << Log::err << "Invalid base texture \"" << name << "\"." << Log::endl;
        pos = textures["_missing"]->pos;
    }

    return getBytesAtPos({pos.x, pos.y}, {pos.z - pos.x, pos.w - pos.y});
}

RawTexData TextureAtlas::getBytesAtPos(glm::ivec2 pos, glm::ivec2 dims) {
    RawTexData data {};
    data.width = dims.x;
    data.height = dims.y;

    auto pixels = new unsigned char[data.width * data.height * 4];

    for (int i = 0; i < data.width * data.height; i++) {
        int xx = pos.x + (i % data.width);
        int yy = pos.y + (i / data.width);

        pixels[i * 4 + 0] = atlasData[xx * 4     + yy * (pixelSize.x * 4)];
        pixels[i * 4 + 1] = atlasData[xx * 4 + 1 + yy * (pixelSize.x * 4)];
        pixels[i * 4 + 2] = atlasData[xx * 4 + 2 + yy * (pixelSize.x * 4)];
        pixels[i * 4 + 3] = atlasData[xx * 4 + 3 + yy * (pixelSize.x * 4)];
    }

    data.data = pixels;

    return data;
}

glm::vec2 TextureAtlas::findImageSpace(int w, int h) {
    for (int j = 0; j < tileSize.y - (h - 1); j++) {
        for (int i = 0; i < tileSize.x - (w - 1); i++) {
            if (empty[j * tileSize.x + i]) {
                bool space = true;

                for (int k = 0; k < h; k++) {
                    for (int l = 0; l < w; l++) {
                        if (!empty[(j + k) * tileSize.x + (i + l)]) {
                            space = false;
                            break;
                        }
                    }

                    if (!space) break;
                }

                if (space) {
                    for (int k = 0; k < h; k++) {
                        for (int l = 0; l < w; l++) {
                            empty[(j + k) * tileSize.x + (i + l)] = false;
                        }
                    }
                    return glm::vec2(i, j);
                }
            }
        }
    }

    return glm::vec2(-1, -1);
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

void TextureAtlas::updateAtlas(int tileX, int tileY, int texWidth, int texHeight, unsigned char *data) {
    int baseX = tileX * 16;
    int baseY = tileY * 16;

    atlasTexture.updateTexture(baseX, baseY, texWidth, texHeight, data);

    for (int i = 0; i < texWidth * texHeight * 4; i++) {
        int xx = (i / 4) % texWidth;
        int yy = (i / 4) / texWidth;
        int of = i % 4;

        atlasData[(baseX + xx + (baseY + yy) * pixelSize.x) * 4 + of] = data[(xx + yy * texWidth) * 4 + of];
    }
}

void TextureAtlas::deleteImage(std::shared_ptr<AtlasRef> ref) {
    // Actually delete the image from the texture (for debugging)

    //auto data = new unsigned char[ref->width * ref->height * 4];
    //
    //for (int i = 0; i < ref->width * ref->height * 4; i++) {
    //    data[i] = 0;
    //}
    //
    //updateAtlas(ref->tileX, ref->tileY, ref->width, ref->height, data);
    //delete[] data;

    textureSlotsUsed -= ref->tileWidth * ref->tileHeight;

    for (float i = ref->tileX; i < ref->tileX + ref->tileWidth; i++) {
        for (float j = ref->tileY; j < ref->tileY + ref->tileHeight; j++) {
            empty[j * tileSize.x + i] = true;
        }
    }
}