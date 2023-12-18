#pragma once

#include "Engine/Component.h"

class ComponentFollowTarget : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;

public:
	void Init(
		std::string targetId,
		glm::vec3 offset
	);

private:
	std::string _targetId;
	std::weak_ptr<MyEngine::GameObject> _target;
	glm::vec3 _offset;
};