//
// Created by aurailus on 06/02/19.
//

#include <catch.hpp>
#include <iostream>
#include "../../../src/generic/network/Serializer.h"

TEST_CASE("Serializer", "[networking]") {

    SECTION("Single Integer") {
        for (int i = 0; i < 20; i++) {

            int int1 = rand();
            std::string target;

            Serializer::encodeInt(target, int1);
            int int2 = Serializer::decodeInt(&(target[0]));

            REQUIRE(int1 == int2);
        }
    }

    SECTION("Single Float") {
        for (int i = 0; i < 20; i++) {

            float float1 = rand() / 1513.0f;
            std::string target;

            Serializer::encodeFloat(target, float1);
            float float2 = Serializer::decodeFloat(&(target[0]));

            REQUIRE(float1 == float2);
        }
    }

    SECTION("Integer Vector") {
        for (int i = 0; i < 5; i++) {

            std::vector<int> integers;
            for (int j = 0; j < 4096; j++) {
                integers.push_back(rand());
            }

            std::string target;

            Serializer::encodeIntVec(target, integers);

            auto outs = Serializer::decodeIntVec(target);

            for (int j = 0; j < integers.size(); j++) {
                REQUIRE(integers[j] == outs[j]);
            }
        }
    }

    SECTION("Float Vector") {
        for (int i = 0; i < 5; i++) {
            std::vector<float> floats;
            for (int j = 0; j < 4096; j++) {
                floats.push_back(rand() / 1513.0f);
            }

            std::string target;

            Serializer::encodeFloatVec(target, floats);

            auto outs = Serializer::decodeFloatVec(target);

            for (int j = 0; j < floats.size(); j++) {
                REQUIRE(floats[j] == outs[j]);
            }
        }
    }

    SECTION("Pascal Style String") {
        std::string str("Hello world what's hanging this is a unit test to determine string serialization works.");

        std::string target;
        Serializer::encodeString(target, str);

        std::string out = Serializer::decodeString(&target[0]);

        REQUIRE(str == out);
    }

    SECTION("Int Vector Shorthands are Interchangable") {
        for (int i = 0; i < 5; i++) {

            std::vector<int> values;

            for (int j = 0; j < 10; j++) {
                values.push_back(rand());
            }

            //Serializing vec, then individual back
            {
                std::string target;
                Serializer::encodeIntVec(target, values);

                std::vector<int> out;
                for (int j = 0; j < 10; j++) {
                    out.push_back(Serializer::decodeInt(&target[j * 4]));
                }

                for (int j = 0; j < values.size(); j++) {
                    REQUIRE(values[j] == out[j]);
                }
            }

            //Serializing individually, then vec back
            {
                std::string target;
                for (int j = 0; j < 10; j++) {
                    Serializer::encodeInt(target, values[j]);
                }

                std::vector<int> out = Serializer::decodeIntVec(target);

                for (int j = 0; j < values.size(); j++) {
                    REQUIRE(values[j] == out[j]);
                }
            }
        }
    }

    SECTION("Float Vector Shorthands are Interchangable") {
        for (int i = 0; i < 5; i++) {

            std::vector<float> values;

            for (int j = 0; j < 10; j++) {
                values.push_back(rand() / 1513.0f);
            }

            //Serializing vec, then individual back
            {
                std::string target;
                Serializer::encodeFloatVec(target, values);

                std::vector<float> out;
                for (int j = 0; j < 10; j++) {
                    out.push_back(Serializer::decodeFloat(&target[j * 4]));
                }

                for (int j = 0; j < values.size(); j++) {
                    REQUIRE(values[j] == out[j]);
                }
            }

            //Serializing individually, then vec back
            {
                std::string target;
                for (int j = 0; j < 10; j++) {
                    Serializer::encodeFloat(target, values[j]);
                }

                std::vector<float> out = Serializer::decodeFloatVec(target);

                for (int j = 0; j < values.size(); j++) {
                    REQUIRE(values[j] == out[j]);
                }
            }
        }
    }
}