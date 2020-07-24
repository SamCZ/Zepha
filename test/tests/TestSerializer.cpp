//
// Created by aurailus on 2020-01-06.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../../src/net/Serializer.h"
#include "../../src/net/Deserializer.h"

TEST_CASE("Serialization", "[net]") {
    Serializer s;

    SECTION("Reference Read") {
        s.append<int>(32);
        s.append<unsigned short>(16);
        s.append<float>(5.2f);
        s.append<glm::vec3>(glm::vec3 {1.f, 2.f, 3.f});
        s.append<std::string>("whatup");

        Deserializer d(s.data);

        int i;
        unsigned short s;
        float f;
        glm::vec3 v3;
        std::string str;

        d.read<int>(i)
         .read<unsigned short>(s)
         .read<float>(f)
         .read<glm::vec3>(v3)
         .read<std::string>(str);

        REQUIRE(i == 32);
        REQUIRE(s == 16);
        REQUIRE(f == 5.2f);
        REQUIRE(v3 == glm::vec3 {1.f, 2.f, 3.f});
        REQUIRE(str == "whatup");
    }

    SECTION("Int") {
        s.append<int>(-32);
        s.append<int>(255);

        Deserializer d(s.data);
        REQUIRE(d.read<int>() == -32);
        REQUIRE(d.read<int>() == 255);
    }

    SECTION("Unsigned Int") {
        s.append<unsigned int>(1);
        s.append<unsigned int>(255);

        Deserializer d(s.data);
        REQUIRE(d.read<unsigned int>() == 1);
        REQUIRE(d.read<unsigned int>() == 255);
    }

    SECTION("Short") {
        s.append<short>(-100);
        s.append<short>(255);

        Deserializer d(s.data);
        REQUIRE(d.read<short>() == -100);
        REQUIRE(d.read<short>() == 255);
    }

    SECTION("Unsigned Short") {
        s.append<unsigned short>(1);
        s.append<unsigned short>(255);

        Deserializer d(s.data);
        REQUIRE(d.read<unsigned short>() == 1);
        REQUIRE(d.read<unsigned short>() == 255);
    }

    SECTION("Float") {
        s.append<float>(1.1f);
        s.append<float>(3.5);
        s.append<float>(-1003.5);

        Deserializer d(s.data);
        REQUIRE(d.read<float>() == 1.1f);
        REQUIRE(d.read<float>() == 3.5);
        REQUIRE(d.read<float>() == -1003.5);
    }

    SECTION("String") {
        s.append<std::string>("hello");
        s.append<std::string>("hi");

        Deserializer d(s.data);
        REQUIRE(d.read<std::string>() == "hello");
        REQUIRE(d.read<std::string>() == "hi");
    }

    SECTION("Vec2") {
        s.append<glm::vec2>({3, 5});
        s.append<glm::vec2>({1.2, -56.1});

        Deserializer d(s.data);
        REQUIRE(d.read<glm::vec2>() == glm::vec2 {3, 5});
        REQUIRE(d.read<glm::vec2>() == glm::vec2 {1.2, -56.1});
    }

    SECTION("IVec2") {
        s.append<glm::ivec2>({3, 5});
        s.append<glm::ivec2>({1, -2});

        Deserializer d(s.data);
        REQUIRE(d.read<glm::ivec2>() == glm::ivec2 {3, 5});
        REQUIRE(d.read<glm::ivec2>() == glm::ivec2 {1, -2});
    }

    SECTION("Vec3") {
        s.append<glm::vec3>({3, 5, 1});
        s.append<glm::vec3>({1.2, -56.1, 0.0});

        Deserializer d(s.data);
        REQUIRE(d.read<glm::vec3>() == glm::vec3 {3, 5, 1});
        REQUIRE(d.read<glm::vec3>() == glm::vec3 {1.2, -56.1, 0.0});
    }

    SECTION("IVec3") {
        s.append<glm::ivec3>({3, 5, 1});
        s.append<glm::ivec3>({1, -2, -7});

        Deserializer d(s.data);
        REQUIRE(d.read<glm::ivec3>() == glm::ivec3 {3, 5, 1});
        REQUIRE(d.read<glm::ivec3>() == glm::ivec3 {1, -2, -7});
    }

    SECTION("Vector<Int>") {
        s.append<std::vector<int>>({1, -2, 5});

        Deserializer d(s.data);
        auto vec = d.read<std::vector<int>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == -2);
        REQUIRE(vec[2] == 5);
    }

    SECTION("Vector<Unsigned int>") {
        s.append<std::vector<unsigned int>>({1, 2, 5});

        Deserializer d(s.data);
        auto vec = d.read<std::vector<unsigned int>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 5);
    }

    SECTION("Vector<Short>") {
        s.append<std::vector<short>>({1, -2, 5});

        Deserializer d(s.data);
        auto vec = d.read<std::vector<short>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == -2);
        REQUIRE(vec[2] == 5);
    }

    SECTION("Vector<Unsigned Short>") {
        s.append<std::vector<unsigned short>>({1, 2, 5});

        Deserializer d(s.data);
        auto vec = d.read<std::vector<unsigned short>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 5);
    }

    SECTION("Vector<Float>") {
        s.append<std::vector<float>>({-1.0f, 2.1, 5.7});

        Deserializer d(s.data);
        auto vec = d.read<std::vector<float>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == -1.0f);
        REQUIRE(vec[1] == 2.1f);
        REQUIRE(vec[2] == 5.7f);
    }

    SECTION("Vector<String>") {
        s.append<std::vector<std::string>>({
            "hi",
            "hello",
            "how are you?"
        });

        Deserializer d(s.data);
        auto vec = d.read<std::vector<std::string>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == "hi");
        REQUIRE(vec[1] == "hello");
        REQUIRE(vec[2] == "how are you?");
    }
}