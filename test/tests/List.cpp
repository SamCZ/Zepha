//
// Created by aurailus on 22/04/19.
//

#include <catch.hpp>
#include <list>

struct IntHolder {
    int i;
};

TEST_CASE("List Iterators are static", "random") {

    std::list<IntHolder> list;

    list.push_back({5});
    auto five = --list.end();

    list.push_back({3});
    auto three = --list.end();

    list.push_back({11});
    auto eleven = --list.end();

    REQUIRE(five->i == 5);
    REQUIRE(three->i == 3);
    REQUIRE(eleven->i == 11);

    list.erase(three);

    REQUIRE(five->i == 5);
    REQUIRE(eleven->i == 11);

    list.erase(eleven);

    REQUIRE(five->i == 5);
}