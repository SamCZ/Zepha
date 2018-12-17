//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_ARRAYTRANS3D_H
#define GLPROJECT_ARRAYTRANS3D_H

#include <vec3.hpp>

class ArrayTrans3D {
    static const int SIZE = 16;

public:
    static void indAssignVec(int ind, glm::vec3* vec) {
        vec->z = ind / (SIZE * SIZE);
        ind -= ((int)vec->z * SIZE * SIZE);
        vec->y = ind / SIZE;
        vec->x = ind % SIZE;
    }

    static glm::vec3* indToVec(int ind) {
        int z = ind / (SIZE * SIZE);
        ind -= (z * SIZE * SIZE);
        int y = ind / SIZE;
        int x = ind % SIZE;
        return new glm::vec3(x, y, z);
    }

    static unsigned int vecToInd(glm::vec3* vec) {
        return vecToInd((int)vec->x, (int)vec->y, (int)vec->z);
    }

    static unsigned int vecToInd(int x, int y, int z) {
        return (unsigned int)(x + SIZE * (y + SIZE * z));
    }
};


#endif //GLPROJECT_ARRAYTRANS3D_H
