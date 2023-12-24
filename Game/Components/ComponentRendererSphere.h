#pragma once

#include "Engine/Component.h"

class ComponentRendererSphere : public MyEngine::Component {
public:
	void Init(rapidjson::Value& serializedData) override;
	void Render(sre::RenderPass& renderPass) override;

private:
	std::shared_ptr<sre::Mesh> _mesh;
	std::shared_ptr<sre::Material> _material;
	std::shared_ptr<sre::Texture> _texture;
	glm::vec3 prev_pos;
};