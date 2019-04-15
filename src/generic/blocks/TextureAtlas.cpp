//
// Created by aurailus on 06/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <cute_files.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include "TextureAtlas.h"

struct TextureRef {
    TextureRef() = default;

    char path[CUTE_FILES_MAX_PATH];
    char name[CUTE_FILES_MAX_FILENAME];
    int width;
    int height;
    int bitDepth;
    unsigned char* texData;
};

TextureAtlas::TextureAtlas() {
    maxTexSize = 0;
}

//The Texture Atlas finds all textures in the specified directory and creates texture pages out of them.
TextureAtlas::TextureAtlas(const char* directory) {

    //Get the max size of a texture on the system and store it in maxTexSize. This variable is the amount of *bytes*
    //wide and tall (to be verified) so it needs to be divided by the amount of bytes 1 pixel takes in the used format.
    //Since we're using RGBA it would be divided by 4.

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << "This GPU's max texture size is: " << maxTexSize / 4 << "px^2." << std::endl;

    //Using cute_files to open the directory and search for files.
    //Filter the files by extension and add all of them to a vector as TextureRefs.
    cf_dir_t dir;
    cf_dir_open(&dir, (std::string(directory) + std::string("/game")).c_str());

    std::list<TextureRef> textureRefs;

    //Load Missing Texture
    auto msg = TextureRef();
    strcpy(msg.path, "../res/tex/_missing.png");
    strcpy(msg.name, "_missing.png");
    msg.texData = stbi_load(msg.path, &msg.width, &msg.height, &msg.bitDepth, 4);
    textureRefs.push_back(msg);

    //Iterate though the files
    while (dir.has_next) {
        cf_file_t file;
        cf_read_file(&dir, &file);

        if (!file.is_dir && strcmp(file.ext, ".png") == 0) {

//            printf("Loading Texture: %s\n", file.name);

            auto ref = TextureRef();
            strcpy(ref.path, file.path);
            strcpy(ref.name, file.name);

            //Load the image and put it into the TextureRef
            ref.texData = stbi_load(file.path, &ref.width, &ref.height, &ref.bitDepth, 4);

            textureRefs.push_back(ref);
        }

        cf_dir_next(&dir);
    }

    cf_dir_close(&dir);

    //Sort by height descending, so that we get the largest images out of the way first. Also that means that images
    //organized by height, decreasing the inefficiency caused by having 1 or two tall images in rows of smaller ones.
    textureRefs.sort([](const TextureRef &a, const TextureRef &b) {
        return (a.height > b.height);
    });

    //Define the texpage width and height
    int pageWidth = 1024;
    int pageHeight = pageWidth;

    //Define that pagedata array and clean the memory it contains
    auto* pageData = new unsigned char[pageWidth*4 * pageHeight];

    for (int i = 0; i < pageWidth * pageHeight; i++) {
        pageData[i * 4 + 0] = 0;
        pageData[i * 4 + 1] = 0;
        pageData[i * 4 + 2] = 0;
        pageData[i * 4 + 3] = 0;
    }

    //Add all of the textures to the textureAtlas and store the UVs in the associative array
    int widthOffset = TEX_PADDING;
    int heightOffset = TEX_PADDING;
    int tallestInRow = 0;

    for (auto i : textureRefs) {
        if (widthOffset + i.width + TEX_PADDING*2 > pageWidth) {
            widthOffset = 0;
            heightOffset += tallestInRow + TEX_PADDING*2;
            tallestInRow = i.height;
        }
        else {
            tallestInRow = std::max(tallestInRow, i.height);
        }

        std::string name = std::string(i.name);
        size_t extensionStart = name.find_last_of('.');
        name = name.substr(0, extensionStart);

        glm::vec4 uv = glm::vec4(widthOffset / (double)pageWidth, heightOffset / (double)pageHeight,
                                (widthOffset + i.width) / (double)pageWidth, (heightOffset + i.height) / (double)pageHeight);

        textures.insert(std::pair<std::string, glm::vec4>(name, uv));

        if (i.width > 0 && i.height > 0) {
            int xx, yy;
            for (int y = -TEX_PADDING; y < i.height + TEX_PADDING; y++) {
                for (int x = -TEX_PADDING; x < i.width + TEX_PADDING; x++) {
                    yy = std::min(std::max(y, 0), i.height - 1);
                    xx = std::min(std::max(x, 0), i.width - 1);

                    pageData[(heightOffset + y)*pageWidth*4 + (widthOffset + x)*4 + 0] = i.texData[(yy * i.width * 4) + (xx * 4) + 0];
                    pageData[(heightOffset + y)*pageWidth*4 + (widthOffset + x)*4 + 1] = i.texData[(yy * i.width * 4) + (xx * 4) + 1];
                    pageData[(heightOffset + y)*pageWidth*4 + (widthOffset + x)*4 + 2] = i.texData[(yy * i.width * 4) + (xx * 4) + 2];
                    pageData[(heightOffset + y)*pageWidth*4 + (widthOffset + x)*4 + 3] = i.texData[(yy * i.width * 4) + (xx * 4) + 3];
                }
            }
        }
        else {
            std::cerr << "Failed to load a texture: " << i.name << std::endl;
        }

        widthOffset += i.width + TEX_PADDING*2;
    }

    stbi_write_png("../textureAtlas.png", pageWidth, pageHeight, 4, pageData, pageWidth*4);
	texture = new Texture();
	texture->load(pageData, pageWidth, pageHeight);
}

Texture* TextureAtlas::getTexture() {
    return texture;
}

glm::vec4 TextureAtlas::getUVs(std::string &texture) {
    if (textures.count(texture) == 0) {
        std::cout << "Texture '" << texture << "' Not found in textureAtlas! Terminating." << std::endl;
        throw std::exception();
    }
    return textures.at(texture);
}
#pragma clang diagnostic pop