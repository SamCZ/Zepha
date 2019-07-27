//
// Created by aurailus on 10/02/19.
//

#include "GUIRect.h"

GUIRect::GUIRect(const std::string &key, GUIComponent *parent) : GUIComponent(key, parent) {}

void GUIRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color) {
    // Single Color Constructor
    // Creates a GUIRect object whose background
    // is a flat color defined by 'color'.

    this->scale = scale;
    this->padding = padding;

    std::vector<EntityVertex> vertices {
            {{0, 0, 0}, {color}, false, {}}, {{0, 1, 0}, {color}, false, {}},
            {{1, 1, 0}, {color}, false, {}}, {{1, 0, 0}, {color}, false, {}}
    };
    std::vector<unsigned int> indices {0, 1, 2, 2, 3, 0};

    auto mesh = new EntityMesh();
    mesh->create(vertices, indices);
    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z, 1});
}

void GUIRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    // Multiple Color Constructor
    // Creates a GUIRect object with a gradient background
    // defined by 'tl', 'tr', 'bl', and 'br'.

    this->scale = scale;
    this->padding = padding;

    std::vector<EntityVertex> vertices {
            {{0, 0, 0}, tl, false, {}}, {{0, 1, 0}, bl, false, {}},
            {{1, 1, 0}, br, false, {}}, {{1, 0, 0}, tr, false, {}}
    };
    std::vector<unsigned int> indices {0, 1, 2, 2, 3, 0};

    auto mesh = new EntityMesh();
    mesh->create(vertices, indices);
    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z, 1});
}

void GUIRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture) {
    // Texture Constructor
    // Creates a GUIRect object with a textured background
    // defined by the 'texture' reference.

    this->scale = scale;
    this->padding = padding;
    this->texture = std::move(texture);

    std::vector<EntityVertex> vertices {
            {{0, 0, 0}, {this->texture->uv.x, this->texture->uv.y, 0, 0}, true, {}},
            {{0, 1, 0}, {this->texture->uv.x, this->texture->uv.w, 0, 0}, true, {}},
            {{1, 1, 0}, {this->texture->uv.z, this->texture->uv.w, 0, 0}, true, {}},
            {{1, 0, 0}, {this->texture->uv.z, this->texture->uv.y, 0, 0}, true, {}}
    };
    std::vector<unsigned int> indices {0, 1, 2, 2, 3, 0};

    auto mesh = new EntityMesh();
    mesh->create(vertices, indices);
    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z, 1});
}
