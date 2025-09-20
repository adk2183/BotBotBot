#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Step {
    std::string primitive;                                  // e.g. "GetCup", "PullShot", "AddWater"
    std::unordered_map<std::string, std::string> params;    // params as strings 
};

struct Recipe {
    std::string name;               // Hot Americano
    std::string glass;              // hot_cup_12oz
    std::vector<Step> steps;        // list of steps 
};

// ---- Parser (implemented in recipe.cpp) ----
Recipe parseRecipeFromYamlFile(const std::string& filepath);
