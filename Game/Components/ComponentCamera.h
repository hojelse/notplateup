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
    void Render(sre::RenderPass&) override;
private:
	sre::Camera* _camera;
};
