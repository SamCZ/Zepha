//
// Created by aurailus on 15/02/19.
//

#ifndef ZEUS_NOISEPARAMS_H
#define ZEUS_NOISEPARAMS_H

struct NoiseParams {

    bool PERLIN_TYPE;

    float NOISE_H_FACTOR;
    float NOISE_V_FACTOR;

    int SAMPLE_H_PRECISION;
    int SAMPLE_V_PRECISION;

    float NOISE_MULTIPLIER;

    //Enum
    const static bool PERLIN_3D = true;
    const static bool PERLIN_2D = false;
};


#endif //ZEUS_NOISEPARAMS_H
