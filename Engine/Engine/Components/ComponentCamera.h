#pragma once

#include "Engine/Component.h"

class ComponentCamera : public MyEngine::Component {
public:
	enum ProjectionType {
		Orthographic,
		Perspective
	};

	void Init(rapidjson::Value& serializedData) override;
	void Update(float) override;
	virtual void KeyEvent(SDL_Event&) override;
private:
	sre::Camera* _camera;

	bool _debugging;
	glm::vec2 _debugMov;
	float _debugMovSpeed = 1;	
};
