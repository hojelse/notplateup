#pragma once

#include "Engine/Component.h"

class ComponentLevelLayout : public MyEngine::Component {
public:
	void Init(rapidjson::Value &serializedData) override;

	void Update(float deltaTime) override;

	void Render(sre::RenderPass &renderPass) override;

	void CreateBox(std::string texture_id, int x, int y, bool is_part_of_layout = true);
private:
	void CreateConsumerIndicator(int id, int x, int y);
};