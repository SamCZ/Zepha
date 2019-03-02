//
// Created by aurailus on 15/02/19.
//

#ifndef ZEUS_INTERPOLATION_H
#define ZEUS_INTERPOLATION_H


class Interpolation {
public:
    static float lerp(float p0, float p1, float factor) {
        return p0 + factor * (p1 - p0);
    }

    static float bilerp(float x0z0, float x1z0, float x0z1, float x1z1, float xfactor, float zfactor) {
        return lerp(lerp(x0z0, x1z0, xfactor), lerp(x0z1, x1z1, xfactor), zfactor);
    }

    static float trilerp(float p000, float p100, float p001, float p101,
                         float p010, float p110, float p011, float p111,
                         float xfactor, float zfactor, float yfactor) {

        return lerp(lerp(lerp(p000, p100, xfactor), lerp(p001, p101, xfactor), zfactor),
                    lerp(lerp(p010, p110, xfactor), lerp(p011, p111, xfactor), zfactor), yfactor);
    }
};


#endif //ZEUS_INTERPOLATION_H
