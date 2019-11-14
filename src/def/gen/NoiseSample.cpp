//
// Created by aurailus on 15/02/19.
//

#include <cmath>
#include "NoiseSample.h"
#include "../../util/Interp.h"
#include "../../util/TransPos.h"

NoiseSample::NoiseSample(int hPrecision, int vPrecision) :
    hPrecision(hPrecision),
    vPrecision(vPrecision),
    data(std::vector<std::vector<std::vector<float>>> {}) {

    data.reserve((unsigned int)hPrecision + 1);

    for (int i = 0; i <= hPrecision; i++) {
        std::vector<std::vector<float>> subdata;
        subdata.reserve((unsigned int)vPrecision + 1);

        for (int j = 0; j <= vPrecision; j++) {
            subdata.emplace_back((unsigned int)hPrecision + 1);
        }

        data.push_back(subdata);
    }
}

void NoiseSample::set(glm::vec3 pos, float value) {
    data[(int)pos.x][(int)pos.y][(int)pos.z] = value;
}

float NoiseSample::get(const glm::vec3& pos) {
    auto xInt = static_cast<int>(pos.x);
    auto yInt = static_cast<int>(pos.y);
    auto zInt = static_cast<int>(pos.z);

    int offsetH = static_cast<int>(static_cast<float>(TransPos::CHUNK_SIZE) / hPrecision);
    int offsetV = static_cast<int>(static_cast<float>(TransPos::CHUNK_SIZE) / vPrecision);

    auto xBase = xInt / offsetH;
    auto yBase = yInt / offsetV;
    auto zBase = zInt / offsetH;

    float xFac = (xInt % offsetH) / ((float)TransPos::CHUNK_SIZE / hPrecision);
    float yFac = (yInt % offsetV) / ((float)TransPos::CHUNK_SIZE / vPrecision);
    float zFac = (zInt % offsetH) / ((float)TransPos::CHUNK_SIZE / hPrecision);

    auto p000 = data[xBase][yBase][zBase];
    auto p100 = data[xBase + 1][yBase][zBase];
    auto p001 = data[xBase][yBase][zBase + 1];
    auto p101 = data[xBase + 1][yBase][zBase + 1];

    if (vPrecision > 1) {
        auto p010 = data[xBase][yBase + 1][zBase];
        auto p110 = data[xBase + 1][yBase + 1][zBase];
        auto p011 = data[xBase][yBase + 1][zBase + 1];
        auto p111 = data[xBase + 1][yBase + 1][zBase + 1];

        return Interp::trilerp(
                p000, p100, p001, p101,
                p010, p110, p011, p111,
                xFac, zFac, yFac
        );
    }
    else {
        return Interp::bilerp(
                p000, p100, p001, p101, xFac, zFac
        );
    }
}

NoiseSample NoiseSample::getSample(noise::module::Module *module, glm::vec3 chunkPos, int hPrecision, int vPrecision, bool flat) {
    NoiseSample s(hPrecision, vPrecision);

    float offsetH = (float)TransPos::CHUNK_SIZE / hPrecision;
    float offsetV = (float)TransPos::CHUNK_SIZE / vPrecision;

    for (int i = 0; i <= hPrecision; i++) {
        for (int j = 0; j <= vPrecision; j++) {
            for (int k = 0; k <= hPrecision; k++) {

                //16s here are constant factor scaling, not to be based on the Chunk size.
                double xCoord = (chunkPos.x * TransPos::CHUNK_SIZE + offsetH * i) / 16;
                double yCoord = (flat) ? 0 : (chunkPos.y * TransPos::CHUNK_SIZE + offsetV * j) / 16;
                double zCoord = (chunkPos.z * TransPos::CHUNK_SIZE + offsetH * k) / 16;

                s.set(glm::vec3(i, j, k), (float)module->GetValue(xCoord, yCoord, zCoord));
            }
        }
    }

    return std::move(s);
}
