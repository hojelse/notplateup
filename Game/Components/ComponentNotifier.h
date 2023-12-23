#pragma once

#include "Engine/Component.h"

class ComponentNotifier : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float delta) override;
	void Render(sre::RenderPass&) override;
public:
	void SetMessage(std::string);

private:
	float _total_duration;
	float _duration_remaining;
	std::string _message;
};
