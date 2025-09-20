// recipe.cpp
#include "recipe.hpp"

#include <yaml-cpp/yaml.h> 
#include <stdexcept>
#include <string>

// Turn a scalar (string/int/double/bool) into std::string.
// If it's not a scalar, return empty string (we only store scalars in params).
static std::string scalarToString(const YAML::Node& n) {
    if (!n || !n.IsScalar()) return "";
    try {
        // yaml-cpp is permissive: numbers/bools convert to strings fine.
        return n.as<std::string>();
    } catch (...) {
        // Fallback (rare): dump exact YAML text
        return YAML::Dump(n);
    }
}

static Step readStep(const YAML::Node& n) {
    if (!n || !n.IsMap())
        throw std::runtime_error("Step must be a map with 'primitive' and optional 'params'");

    Step s;

    // primitive (required, scalar)
    const auto prim = n["primitive"];
    if (!prim || !prim.IsScalar())
        throw std::runtime_error("Step missing scalar 'primitive'");
    s.primitive = prim.as<std::string>();

    // params (optional, map of scalars)
    const auto params = n["params"];
    if (params) {
        if (!params.IsMap())
            throw std::runtime_error("Step 'params' must be a map");
        for (auto it = params.begin(); it != params.end(); ++it) {
            const std::string key = it->first.as<std::string>();
            s.params[key] = scalarToString(it->second);  // store everything as string
        }
    }

    return s;
}

Recipe parseRecipeFromYamlFile(const std::string& filepath) {
    YAML::Node root = YAML::LoadFile(filepath);
    if (!root || !root.IsMap())
        throw std::runtime_error("Recipe YAML root must be a map: " + filepath);

    Recipe r;

    // name (required)
    const auto name = root["name"];
    if (!name || !name.IsScalar())
        throw std::runtime_error("Recipe missing 'name': " + filepath);
    r.name = name.as<std::string>();

    // glass (required)
    const auto glass = root["glass"];
    if (!glass || !glass.IsScalar())
        throw std::runtime_error("Recipe missing 'glass': " + filepath);
    r.glass = glass.as<std::string>();

    // steps (required)
    const auto steps = root["steps"];
    if (!steps || !steps.IsSequence())
        throw std::runtime_error("Recipe missing 'steps' sequence: " + filepath);

    r.steps.reserve(steps.size());
    for (const auto& sn : steps) r.steps.push_back(readStep(sn));

    return r;
}
