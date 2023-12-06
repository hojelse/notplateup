#pragma once

#include "Engine/Component.h"

class ComponentRendererMesh : public MyEngine::Component {
public:
	void Init(rapidjson::Value& serializedData) override;
	void Update(float deltaTime) override;
	void Render(sre::RenderPass& renderPass) override;

private:
	std::shared_ptr<sre::Mesh> _mesh;
	std::shared_ptr<sre::Material> _material;
	std::shared_ptr<sre::Texture> _texture;
};