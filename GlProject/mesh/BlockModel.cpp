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