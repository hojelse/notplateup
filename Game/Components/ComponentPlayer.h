#pragma once

#include "Engine/Component.h"

class ComponentPlayer : public MyEngine::Component {
public:
	void Init(rapidjson::Value& serializedData) override;
	void Update(float) override;
	void Render(sre::RenderPass& renderPass) override;
private:
};
