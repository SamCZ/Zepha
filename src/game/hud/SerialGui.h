//
// Created by aurailus on 2019-11-03.
//

#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <cerrno>
#include <cstdlib>
#include <stdexcept>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <sol2/sol.hpp>
#include "../../util/Any.h"

#include <iostream>

namespace SerialGui {
    class Element {
    public:
        Element(const std::string& type, const std::string& key) : type(type), key(key) {};

        std::string type;
        std::string key;

        void addTrait(const std::string& key, const Any& any) {
            traits.emplace(key, any);
        }

        template <typename T> const T& get(const std::string& key) const {
            if (!traits.count(key)) throw std::logic_error("Key missing from table");
            return traits.at(key).get<T>();
        }

        template <typename T> const T& get_or(const std::string& key, const T& other) const noexcept {
            if (!traits.count(key)) return other;
            return traits.at(key).get_or<T>(other);
        }

        template <typename T> const bool has(const std::string& key) const noexcept {
            if (!traits.count(key)) return false;
            return traits.at(key).is<T>();
        }

        std::array<std::function<void(bool, glm::ivec2)>, 3> callbacks = {{nullptr, nullptr, nullptr}};
        std::vector<SerialGui::Element> children {};
    private:
        std::map<std::string, Any> traits {};
    };

    const float SCALE_MODIFIER = 3;

    namespace {
        static std::vector<std::string> split(const std::string& value, unsigned int targetCount = 0) {
            std::vector<std::string> vec {};

            if (value == "") throw std::runtime_error("expected one or more values to split");

            size_t count = std::count(value.begin(), value.end(), ' ');
            if (count + 1 > targetCount) throw std::runtime_error("expected less values");

            // Split the values into the vector.
            size_t begin = 0;
            for (int i = 0; i < count; i++) {
                size_t end = value.find(' ', begin);
                vec.push_back(value.substr(begin, end - begin));
                begin = end + 1;
            }
            vec.push_back(value.substr(begin));

            // Duplicate values until we meet or surpass the expected values.
            while (vec.size() < targetCount) for (auto& v: vec) vec.push_back(v);
            if (vec.size() > targetCount) throw std::runtime_error("values are not a division of expectation");

            return std::move(vec);
        }

        static double toDouble(const std::string& input) {
            char* e;
            errno = 0;

            if (input.find('%') == input.length() - 1) {
                double v = std::strtod(input.substr(0, input.find("%")).c_str(), &e) / 100;
                if (*e != '\0' || errno != 0) throw std::runtime_error("error decoding num from string");
                return v - 10.f; // Percentages are going to be stored in negatives. Ew.
            }

            double v = round(std::strtod(input.c_str(), &e));
            if (*e != '\0' || errno != 0) throw std::runtime_error("error decoding num from string");
            return v;
        }

        static double convertNum(float input, unsigned int multiple) {
            if (input >= -20 && input < 0) {
                if (!multiple) return input + 10;
                else return (((input + 10) * multiple / SCALE_MODIFIER) * SCALE_MODIFIER);
            }

            return input * SCALE_MODIFIER;
        }
    }

    template <typename T> static T calcNumbers(const T in, glm::ivec2 multiple = {}) {};

    template <typename T> static T get(const SerialGui::Element& elem, const std::string& req, glm::ivec2 multiple = {}) {
        if (!elem.has<T>(req)) return T{};
        return calcNumbers<T>(elem.get<T>(req), multiple);
    }

    template <> glm::vec2 calcNumbers<glm::vec2>(const glm::vec2 in, glm::ivec2 multiple) {
        return {convertNum(in.x, multiple.x), convertNum(in.y, multiple.y)};
    }

    template <> glm::vec4 calcNumbers<glm::vec4>(const glm::vec4 in, glm::ivec2 multiple) {
        return {convertNum(in.x, multiple.x), convertNum(in.y, multiple.y), convertNum(in.z, multiple.x), convertNum(in.w, multiple.y)};
    }
};