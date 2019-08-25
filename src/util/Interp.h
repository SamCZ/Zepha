//
// Created by aurailus on 15/02/19.
//

#pragma once


class Interp {
public:
    static inline float lerp(float p0, float p1, float factor) {
        return p0 + factor * (p1 - p0);
    }

    static inline float bilerp(float x0z0, float x1z0, float x0z1, float x1z1, float xfactor, float zfactor) {
        auto p0 = (x0z0 + xfactor * (x1z0 - x0z0));
        auto p1 = (x0z1 + xfactor * (x1z1 - x0z1));
        return p0 + zfactor * (p1 - p0);
    }

    static inline float trilerp(float p000, float p100, float p001, float p101,
                         float p010, float p110, float p011, float p111,
                         float xfactor, float zfactor, float yfactor) {

        auto p00 = (p000 + xfactor * (p100 - p000));
        auto p10 = (p001 + xfactor * (p101 - p001));
        auto p01 = (p010 + xfactor * (p110 - p010));
        auto p11 = (p011 + xfactor * (p111 - p011));

        auto bl = (p00 + zfactor * (p10 - p00));
        auto tl = (p01 + zfactor * (p11 - p01));

        return bl + yfactor * (tl - bl);
    }
};

