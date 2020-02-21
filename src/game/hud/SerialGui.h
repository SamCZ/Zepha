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

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace SerialGui {
    struct Elem {
        std::string type;
        std::string key;
        std::map<std::string, std::string> tokens;
        std::vector<SerialGui::Elem> children;
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

        static double toDouble(const std::string& input, unsigned int multiple) {
            char* e;
            errno = 0;

            if (input.find("px") == input.length() - 2) {
                double v = round(std::strtod(input.substr(0, input.find("px")).c_str(), &e));
                if (*e != '\0' || errno != 0) throw std::runtime_error("error decoding num from string");
                return v * SCALE_MODIFIER;
            }

            if (input.find('%') == input.length() - 1) {
                double v = std::strtod(input.substr(0, input.find("%")).c_str(), &e) / 100;
                if (*e != '\0' || errno != 0) throw std::runtime_error("error decoding num from string");

                if (!multiple) return v;
                return round(v * multiple / SCALE_MODIFIER) * SCALE_MODIFIER;
            }

            double v = std::strtod(input.c_str(), &e);
            if (*e != '\0' || errno != 0) throw std::runtime_error("error decoding num from string");
            return v;
        }
    }

    template <typename T> static T deserialize(const std::string& in, glm::ivec2 multiple = {}) {};

    template <typename T> static T deserializeToken(const std::map<std::string, std::string>& tokens,
            const std::string& req, glm::ivec2 multiple = {}) {
        if (!tokens.count(req)) return T{};
        return deserialize<T>(tokens.at(req), multiple);
    }

    template <> glm::vec2 deserialize<glm::vec2>(const std::string& in, glm::ivec2 multiple) {
        auto tokens = split(in, 2);
        return {toDouble(tokens[0], multiple.x), toDouble(tokens[1], multiple.y)};
    }

    template <> glm::vec4 deserialize<glm::vec4>(const std::string& in, glm::ivec2 multiple) {
        auto tokens = split(in, 4);
        return {toDouble(tokens[0], multiple.x), toDouble(tokens[1], multiple.y),
                toDouble(tokens[2], multiple.x), toDouble(tokens[3], multiple.y)};
    }
};