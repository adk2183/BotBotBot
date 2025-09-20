#include "recipe.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

// Simulated primitive functions
void GetCup(const std::unordered_map<std::string, std::string>& params) {
    std::cout << "[Robot] Picking up a " << params.at("size") << " cup.\n";
}

void PullShot(const std::unordered_map<std::string, std::string>& params) {
    std::cout << "[Robot] Pulling a " << params.at("strength") << " espresso shot.\n";
    std::this_thread::sleep_for(std::chrono::seconds(3)); // simulate time
}

void AddWater(const std::unordered_map<std::string, std::string>& params) {
    std::cout << "[Robot] Adding " << params.at("amount_ml") << "ml of hot water.\n";
}

void AddMilk(const std::unordered_map<std::string, std::string>& params) {
    std::cout << "[Robot] Adding " << params.at("amount_ml") << "ml of milk.\n";
}

void AddFoam(const std::unordered_map<std::string, std::string>& params) {
    std::cout << "[Robot] Adding milk foam.\n";
}

// Primitive dispatcher
void executeStep(const Step& step) {
    static const std::unordered_map<std::string, void(*)(const std::unordered_map<std::string, std::string>&)> dispatch = {
        {"GetCup",   GetCup},
        {"PullShot", PullShot},
        {"AddWater", AddWater},
        {"AddMilk",  AddMilk},
        {"AddFoam",  AddFoam}
    };

    auto it = dispatch.find(step.primitive);
    if (it != dispatch.end()) {
        it->second(step.params);
    } else {
        std::cerr << "[Error] Unknown primitive: " << step.primitive << "\n";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <recipe.yaml>\n";
        return 1;
    }

    try {
        Recipe r = parseRecipeFromYamlFile(argv[1]);

        std::cout << "=== Starting Recipe: " << r.name << " ===\n";
        std::cout << "Glass type: " << r.glass << "\n\n";

        for (size_t i = 0; i < r.steps.size(); ++i) {
            const auto& step = r.steps[i];
            std::cout << "Step " << i+1 << ": " << step.primitive << "\n";
            executeStep(step);
            std::this_thread::sleep_for(std::chrono::seconds(1)); // pause between steps
        }

        std::cout << "\n[Robot] Finished making: " << r.name << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error parsing or executing recipe: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
