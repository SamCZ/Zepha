//
// Created by aurailus on 04/04/19.
//

#include <catch.hpp>
#include "../../src/world/Dimension.h"

TEST_CASE("Dimension", "[world]") {

    Dimension d;

//    SECTION("Inputting") {
//        SECTION("[0, 0, 0]") {
//            auto c = new BlockChunk();
//            d.addChunk({0, 0, 0}, c);
//
//            auto o = (*(*d.getRegion({0, 0, 0}))[0])[0];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[1, 2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({1, 2, 3}, c);
//
//            auto o = (*(*d.getRegion({0, 0, 0}))[0])[TransPos::chunkIndFromVec({1, 2, 3})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[9, 2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({9, 2, 3}, c);
//
//            auto o = (*(*d.getRegion({0, 0, 0}))[TransPos::mapBlockIndFromVec({1, 0, 0})])[TransPos::chunkIndFromVec({1, 2, 3})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[66, 2, 16]") {
//            auto c = new BlockChunk();
//            d.addChunk({66, 2, 16}, c);
//
//            auto o = (*(*d.getRegion({1, 0, 0}))[TransPos::mapBlockIndFromVec({0, 0, 2})])[TransPos::chunkIndFromVec({2, 2, 0})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[-1, -2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({-1, -2, 3}, c);
//
//            auto o = (*(*d.getRegion({-1, -1, 0}))[TransPos::mapBlockIndFromVec({7, 7, 0})])[TransPos::chunkIndFromVec({7, 6, 3})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//
//        SECTION("[-1, -2, -3]") {
//            auto c = new BlockChunk();
//            d.addChunk({-1, -2, -3}, c);
//
//            auto o = (*(*d.getRegion({-1, -1, -1}))[TransPos::mapBlockIndFromVec({7, 7, 7})])[TransPos::chunkIndFromVec({7, 6, 5})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[66, -2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({66, -2, 3}, c);
//
//            auto o = (*(*d.getRegion({1, -1, 0}))[TransPos::mapBlockIndFromVec({0, 7, 0})])[TransPos::chunkIndFromVec({2, 6, 3})];
//
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//    }
//
//    SECTION("Retrieving") {
//        SECTION("[0, 0, 0]") {
//            auto c = new BlockChunk();
//            d.addChunk({0, 0, 0}, c);
//
//            auto o = d.getChunk({0, 0, 0});
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[1, 2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({1, 2, 3}, c);
//
//            auto o = d.getChunk({1, 2, 3});
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[-1, -2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({-1, -2, 3}, c);
//
//            auto o = d.getChunk({-1, -2, 3});
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[-1, -2, -3]") {
//            auto c = new BlockChunk();
//            d.addChunk({-1, -2, -3}, c);
//
//            auto o = d.getChunk({-1, -2, -3});
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//
//        SECTION("[1026, -2, 3]") {
//            auto c = new BlockChunk();
//            d.addChunk({1026, -2, 3}, c);
//
//            auto o = d.getChunk({1026, -2, 3});
//            REQUIRE(o != nullptr);
//            REQUIRE(c == o);
//        }
//    }
}