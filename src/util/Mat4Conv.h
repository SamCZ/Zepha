//
// Created by aurailus on 25/08/19.
//

#pragma once

#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>
#include <glm/glm.hpp>

namespace MatConv {
    static inline aiMatrix4x4 GLMMat4ToAi(const glm::mat4 mat) {
        return {mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                mat[3][0], mat[3][1], mat[3][2], mat[3][3]};
    }

    static inline glm::mat4 AiToGLMMat4(const aiMatrix4x4 &in_mat) {
        glm::mat4 tmp;

        tmp[0][0] = in_mat.a1;
        tmp[1][0] = in_mat.b1;
        tmp[2][0] = in_mat.c1;
        tmp[3][0] = in_mat.d1;

        tmp[0][1] = in_mat.a2;
        tmp[1][1] = in_mat.b2;
        tmp[2][1] = in_mat.c2;
        tmp[3][1] = in_mat.d2;

        tmp[0][2] = in_mat.a3;
        tmp[1][2] = in_mat.b3;
        tmp[2][2] = in_mat.c3;
        tmp[3][2] = in_mat.d3;

        tmp[0][3] = in_mat.a4;
        tmp[1][3] = in_mat.b4;
        tmp[2][3] = in_mat.c4;
        tmp[3][3] = in_mat.d4;

        return tmp;
    }

    static inline aiMatrix3x3 GLMMat3ToAi(const glm::mat3 mat) {
        return {mat[0][0], mat[0][1], mat[0][2],
                mat[1][0], mat[1][1], mat[1][2],
                mat[2][0], mat[2][1], mat[2][2]};
    }

    static inline glm::mat3 AiToGLMMat3(const aiMatrix3x3 &in_mat) {
        glm::mat3 tmp;

        tmp[0][0] = in_mat.a1;
        tmp[1][0] = in_mat.b1;
        tmp[2][0] = in_mat.c1;

        tmp[0][1] = in_mat.a2;
        tmp[1][1] = in_mat.b2;
        tmp[2][1] = in_mat.c2;

        tmp[0][2] = in_mat.a3;
        tmp[1][2] = in_mat.b3;
        tmp[2][2] = in_mat.c3;

        return tmp;
    }
}