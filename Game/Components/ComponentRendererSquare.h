#pragma once

#include "Engine/Component.h"

class ComponentRendererSquare : public MyEngine::Component  {
public:
	void Init(rapidjson::Value& serializedData) override;
	void Init(int texture_id);
	void Update(float deltaTime) override;
	void Render(sre::RenderPass& renderPass) override;
	int GetId() { return _id; }

private:
	int _id;
	std::shared_ptr<sre::Mesh> _mesh;
	std::shared_ptr<sre::Material> _material;
	std::shared_ptr<sre::Texture> _texture;
	float _x_rotation;
	float _y_rotation;
};
