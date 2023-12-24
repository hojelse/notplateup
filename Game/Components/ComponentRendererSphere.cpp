#include "ComponentRendererSphere.h"

#include "glm/gtx/transform.hpp"
#include "ComponentController.h"
#include <glm/gtx/matrix_decompose.hpp>

void ComponentRendererSphere::Init(rapidjson::Value &serializedData)
{
	_mesh = sre::Mesh::create()
				.withSphere()
				.build();
	_material = sre::Shader::getUnlit()->createMaterial();

	_texture = sre::Texture::create()
		.withFile("data/level0.png")
		.withGenerateMipmaps(false)
		.withFilterSampling(false)
		.build();

	_material->setTexture(_texture);
}

void ComponentRendererSphere::Render(sre::RenderPass &renderPass)
{
    auto controller = GetGameObject().lock()->FindComponent<ComponentController>();
    auto angle = controller.lock()->GetAngle();
	auto go = GetGameObject().lock();
    auto rotated = glm::rotate(
            go->transform,
            angle,
            glm::vec3(0, 0, 1));
    auto scaled = glm::scale(rotated, {0.5,0.5,1});
    auto translated = glm::translate(scaled, {0,0,0.5});
	renderPass.draw(
		_mesh,
		translated,
		_material
	);
}