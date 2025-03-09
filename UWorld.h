#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "UObject.h"
#include "ULevel.h"

class UWorld : public UObject {
public:
    void AddLevel(std::shared_ptr<ULevel> level) {
        Levels.push_back(level);
    }

    void Tick(float DeltaTime) {
        for (auto& level : Levels) {
            if (level) {
                level->Tick(DeltaTime);
            }
        }
    }

    const std::vector<std::shared_ptr<ULevel>>& GetLevels() const {
        return Levels;
    }

private:
    std::vector<std::shared_ptr<ULevel>> Levels;
};
