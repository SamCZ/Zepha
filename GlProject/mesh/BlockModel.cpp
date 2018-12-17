//
// Created by aurailus on 04/12/18.
//

#include <utility>
#include "BlockModel.h"

BlockModel::BlockModel(bool culls, bool visible) {
    this->nullModel = true;
    this->culls = culls;
    this->visible = visible;
}

BlockModel::BlockModel(vector<MeshPart*> leftFaces, vector<MeshPart*> rightFaces, vector<MeshPart*> topFaces,
                       vector<MeshPart*> bottomFaces, vector<MeshPart*> frontFaces, vector<MeshPart*> backFaces,
                       vector<MeshPart*> noCulledFaces, bool culls, bool visible) {

    this->leftFaces     = std::move(leftFaces);
    this->rightFaces    = std::move(rightFaces);
    this->topFaces      = std::move(topFaces);
    this->bottomFaces   = std::move(bottomFaces);
    this->frontFaces    = std::move(frontFaces);
    this->backFaces     = std::move(backFaces);
    this->noCulledFaces = std::move(noCulledFaces);

    this->culls = culls;
    this->visible = visible;
}

BlockModel::BlockModel(MeshPart* leftFace, MeshPart* rightFace, MeshPart* topFace, MeshPart* bottomFace,
                       MeshPart* frontFace, MeshPart* backFace, MeshPart* noCulledFace, bool culls, bool visible) {

    this->leftFaces = vector<MeshPart*>();
    if (leftFace != nullptr)
        this->leftFaces.push_back(leftFace);

    this->rightFaces = vector<MeshPart*>();
    if (rightFace != nullptr)
        this->rightFaces.push_back(rightFace);

    this->topFaces = vector<MeshPart*>();
    if (topFace != nullptr)
        this->topFaces.push_back(topFace);

    this->bottomFaces = vector<MeshPart*>();
    if (bottomFace != nullptr)
        this->bottomFaces.push_back(bottomFace);

    this->backFaces = vector<MeshPart*>();
    if (backFace != nullptr)
        this->backFaces.push_back(backFace);

    this->frontFaces = vector<MeshPart*>();
    if (frontFace != nullptr)
        this->frontFaces.push_back(frontFace);

    this->noCulledFaces = vector<MeshPart*>();
    if (noCulledFace != nullptr)
        this->noCulledFaces.push_back(noCulledFace);

    this->culls = culls;
    this->visible = visible;
}

BlockModel::~BlockModel() {
    for (MeshPart* m : leftFaces) { delete m; }
    leftFaces.clear();

    for (MeshPart* m : rightFaces) { delete m; }
    rightFaces.clear();

    for (MeshPart* m : topFaces) { delete m; }
    topFaces.clear();

    for (MeshPart* m : bottomFaces) { delete m; }
    bottomFaces.clear();

    for (MeshPart* m : frontFaces) { delete m; }
    frontFaces.clear();

    for (MeshPart* m : backFaces) { delete m; }
    backFaces.clear();

    for (MeshPart* m : noCulledFaces) { delete m; }
    noCulledFaces.clear();
}

//Static methods to generate a basic cube BlockModel.
//These are just convenience functions.

BlockModel* BlockModel::Square(const char *tex, TextureAtlas* atlas) {
    return BlockModel::Square(tex, tex, tex, tex, tex, tex, atlas);
}

BlockModel* BlockModel::Square(const char* top, const char* bottom, const char* front, const char* back, const char* left, const char* right, TextureAtlas* atlas) {
    Vertex* leftVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
    };
    auto* leftInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* leftPart = new MeshPart(leftVerts, 4, leftInds, 6, left, atlas);

    Vertex* rightVerts = new Vertex[4] {
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
    };
    auto* rightInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* rightPart = new MeshPart(rightVerts, 4, rightInds, 6, right, atlas);

    Vertex* topVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
    };
    auto* topInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* topPart = new MeshPart(topVerts, 4, topInds, 6, top, atlas);

    Vertex* bottomVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
    };
    auto* bottomInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* bottomPart = new MeshPart(bottomVerts, 4, bottomInds, 6, bottom, atlas);

    Vertex* frontVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
    };
    auto* frontInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* frontPart = new MeshPart(frontVerts, 4, frontInds, 6, front, atlas);

    Vertex* backVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
    };
    auto* backInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* backPart = new MeshPart(backVerts, 4, backInds, 6, back, atlas);

    auto* model = new BlockModel(leftPart, rightPart, topPart, bottomPart, frontPart, backPart, nullptr, true, true);

    return model;
}