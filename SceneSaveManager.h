#pragma once
#include "Types.h"
#include "Vector.h"
#include <fstream>
#include <iostream>
#include "include/nlohmann/json.hpp"
using json = nlohmann::ordered_json;

class SceneSaveManager {
public:
	struct PrimitiveData {
		uint32 UUID;
		FVector Location;
		FVector Rotation;
		FVector Scale;
		std::string Type;

		json VectortoJson(const FVector& data) const {
			return json{ data.X, data.Y, data.Z };
		}

		json toJson() const {
			json j;
			j["Location"] = VectortoJson(Location);
			j["Rotation"] = VectortoJson(Rotation);
			j["Scale"] = VectortoJson(Scale);
			j["Type"] = Type;
			return j;
		}
	};
	uint32 Version = 1;
	uint32 NextUUID;
	TArray<PrimitiveData> SceneData;

public:
	json toJson() const {
		json j;
		j["Version"] = Version;
		j["NextUUID"] = NextUUID;
		json primitivesJson = json::object();
		for (const auto& primitiveData : SceneData) {
			primitivesJson[std::to_string(primitiveData.UUID)] = primitiveData.toJson();
		}
		j["Primitives"] = primitivesJson;
		return j;
	}

	void Save(const char* filename) const {
		std::string fileName = std::string(filename) + ".scene";

		json j = toJson();
		std::ofstream file(fileName);
		if (file.is_open()) {
			file << j.dump(4);
			file.close();
		}
	}

	bool Load(const char* filename) {
		std::string fileName = std::string(filename) + ".scene";
		std::ifstream file(fileName);
		if (file.is_open()) {
			json j;
			file >> j;
			file.close();

			Version = j["Version"];
			NextUUID = j["NextUUID"];
			SceneData.clear();

			for (auto it = j["Primitives"].begin(); it != j["Primitives"].end(); ++it) {
				PrimitiveData primitive;
				primitive.UUID = std::stoi(it.key());
				primitive.Location = FVector(it.value()["Location"][0], it.value()["Location"][1], it.value()["Location"][2]);
				primitive.Rotation = FVector(it.value()["Rotation"][0], it.value()["Rotation"][1], it.value()["Rotation"][2]);
				primitive.Scale = FVector(it.value()["Scale"][0], it.value()["Scale"][1], it.value()["Scale"][2]);
				primitive.Type = FString(it.value()["Type"].get<std::string>());
				SceneData.push_back(primitive);
			}
			return true;
		}
		else {
			return false;
		}
	}
};
