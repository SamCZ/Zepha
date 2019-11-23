//
// Created by aurailus on 15/02/19.
//

#pragma once

namespace Interp {
    const constexpr inline float lerp(float p0, float p1, float factor) {
        return p0 + factor * (p1 - p0);
    }

    const constexpr inline float bilerp(float x0z0, float x1z0, float x0z1, float x1z1, float xfactor, float zfactor) {
        const float p0 = (x0z0 + xfactor * (x1z0 - x0z0));
        return p0 + zfactor * ((x0z1 + xfactor * (x1z1 - x0z1)) - p0);
    }

    const constexpr inline float trilerp(const float p000, const float p100, const float p001, const float p101,
                                         const float p010, const float p110, const float p011, const float p111,
                                         const float xfactor, const float zfactor, const float yfactor) {

        const float p00 = (p000 + xfactor * (p100 - p000));
        const float p01 = (p010 + xfactor * (p110 - p010));
        const float bl = (p00 + zfactor * ((p001 + xfactor * (p101 - p001)) - p00));
        return bl + yfactor * ((p01 + zfactor * ((p011 + xfactor * (p111 - p011)) - p01)) - bl);
    }
}

