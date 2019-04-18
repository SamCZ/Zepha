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

void DynamicAtlas::createMissingTexture() {
    auto *data = new unsigned char[16 * 4 * 16];
    for (int i = 0; i < 16 * 16; i++) {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;

        if ((i % 16 < 8) ^ ((i / 16) < 8)) {
            r = 255;
            b = 255;
        }

        data[i * 4 + 0] = r;
        data[i * 4 + 1] = g;
        data[i * 4 + 2] = b;
        data[i * 4 + 3] = 255;
    }
    addTexture(data, "_missing", 16, 16);
}

//Height is optional and defaults to 0
DynamicAtlas::DynamicAtlas(unsigned int width, unsigned int height) {
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
//    std::cout << "This GPU's max texture size is: " << maxTexSize / 4 << "px^2." << std::endl;
//
//    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
//    std::cout << "This GPU supports " << texUnits << " texture units." << std::endl;

    height = ((height == 0) ? width : height);
    width = width;

    this->width = width;
    this->height = height;

    empty = std::vector<bool>((width / 16) * (height / 16), true);

    auto* data = new unsigned char[width*4 * height];
    for (int i = 0; i < width * height; i++) {
        data[i * 4 + 0] = 0;
        data[i * 4 + 1] = 0;
        data[i * 4 + 2] = 0;
        data[i * 4 + 3] = 0;
    }

    t.loadFromBytes(data, width, height);

    createMissingTexture();

    delete[] data;
}

void DynamicAtlas::loadFromDirectory(std::string dirStr) {
    cf_dir_t dir;
    cf_dir_open(&dir, (dirStr).c_str());

    while (dir.has_next) {
        cf_file_t file;
        cf_read_file(&dir, &file);

        if (!file.is_dir && strcmp(file.ext, ".png") == 0) {
            int width, height;
            unsigned char* data = stbi_load(file.path, &width, &height, nullptr, 4);
            addTexture(data, std::string(file.name).substr(0, std::string(file.name).size() - 4), width, height);
            delete[] data;
        }

        cf_dir_next(&dir);
    }
    cf_dir_close(&dir);
}

glm::vec2 DynamicAtlas::findSpace(int w, int h) {
    for (int j = 0; j < (height / 16) - (h - 1); j++) {
        for (int i = 0; i < (width / 16) - (w - 1); i++) {
            if (empty[j * (width / 16) + i]) {
                bool space = true;

                for (int k = 0; k < h; k++) {
                    for (int l = 0; l < w; l++) {
                        if (!empty[(j + k) * (width / 16) + (i + l)]) {
                            space = false;
                            break;
                        }
                    }

                    if (!space) break;
                }

                if (space) {
                    for (int k = 0; k < h; k++) {
                        for (int l = 0; l < w; l++) {
                            empty[(j + k) * (width / 16) + (i + l)] = false;
                        }
                    }
                    return glm::vec2(i, j);
                }
            }
        }
    }

    return glm::vec2(-1, -1);
}

std::shared_ptr<AtlasRef> DynamicAtlas::addTexture(unsigned char* data, std::string name, int pixelWidth, int pixelHeight) {
    auto ref = std::make_shared<AtlasRef>();

    ref->name = name;
    ref->width = pixelWidth;
    ref->height = pixelHeight;

    int tileWidth = (int)std::ceil(pixelWidth / 16.0f);
    int tileHeight = (int)std::ceil(pixelHeight / 16.0f);

    ref->tileWidth = tileWidth;
    ref->tileHeight = tileHeight;

    auto space = findSpace(tileWidth, tileHeight);

    if (space.x < 0) {
        std::cerr << "Failed to find space in dynamic atlas." << std::endl;
        return nullptr;
    }

    ref->tileX = (int)space.x;
    ref->tileY = (int)space.y;

    t.updateTexture((int)space.x * 16, (int)space.y * 16, pixelWidth, pixelHeight, data);

    ref->uv = {(space.x * 16) / width, (space.y * 16) / height,
               (space.x * 16 + pixelWidth) / width, (space.y * 16 + pixelHeight) / height};

    textures.insert({name, ref});
    return ref;
}

Texture &DynamicAtlas::getTexture() {
    return t;
}

glm::vec4 DynamicAtlas::getTextureUVs(std::string& name) {
    if (!textures.count(name)) {
        std::cerr << "Invalid texture name (at DynamicAtlas.cpp line " << __LINE__ << "): " << name << std::endl;
        return {0, 0, 0, 0};
    }
    return textures.at(name)->uv;
}

DynamicAtlas::~DynamicAtlas() = default;