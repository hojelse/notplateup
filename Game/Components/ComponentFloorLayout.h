#pragma once

#include "Engine/Component.h"

class ComponentFloorLayout : public MyEngine::Component {
public:
	void Init(rapidjson::Value &serializedData) override;

	void Update(float deltaTime) override;

	void Render(sre::RenderPass &renderPass) override;

private:
	void CreateTile(std::string texture_id, int x, int y, bool is_floor);
};