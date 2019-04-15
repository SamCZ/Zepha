//
// Created by aurailus on 04/12/18.
//

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

BlockModel* BlockModel::from_lua_def(sol::table model, sol::table textures, TextureAtlas* atlas, bool visible, bool culls) {
    vector<MeshPart*> leftFaces, rightFaces, topFaces, bottomFaces, frontFaces, backFaces, noCulledFaces;

    for (auto meshPartPair : model) {
        auto meshPart = (sol::table)meshPartPair.second;

        std::string face = meshPart.get_or<std::string>("face", "nocull");
        int tex = (int)meshPart.get_or<float>("tex", 0);

        auto pointsOptional = meshPart.get<sol::optional<sol::table>>("points");
        if (!pointsOptional) break;
        sol::table points = *pointsOptional;

        int texturesLength = textures.size();

        std::string texture = textures.get_or<std::string>(min(tex, texturesLength), "_missing");

        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 1; i <= points.size()/5; i++) {
            int o = (i-1) * 5 + 1;

            glm::vec3 pos((float)points[o], (float)points[o+1], (float)points[o+2]);
            glm::vec2 tex((float)points[o+3], (float)points[o+4]);

            vertices.push_back({pos, {0, 0, 0}, tex, {0, 0}});
        }

        int ind = 0;
        for (int i = 1; i <= points.size()/20; i++) {
            indices.push_back(ind);
            indices.push_back(ind + 1);
            indices.push_back(ind + 2);
            indices.push_back(ind + 2);
            indices.push_back(ind + 3);
            indices.push_back(ind);
            ind += 4;
        }

        auto mp = new MeshPart(std::move(vertices), std::move(indices), texture, atlas);

        if      (face == "top")    topFaces.push_back(mp);
        else if (face == "bottom") bottomFaces.push_back(mp);
        else if (face == "left")   leftFaces.push_back(mp);
        else if (face == "right")  rightFaces.push_back(mp);
        else if (face == "front")  frontFaces.push_back(mp);
        else if (face == "back")   backFaces.push_back(mp);
        else if (face == "nocull") noCulledFaces.push_back(mp);
    }

    return new BlockModel(leftFaces, rightFaces, topFaces, bottomFaces, frontFaces, backFaces, noCulledFaces, culls, visible);
}