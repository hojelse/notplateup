#include "ComponentRendererSphere.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <unistd.h>
#include <cmath>

void ComponentRendererSphere::Init(rapidjson::Value &serializedData)
{
	// GetGameObject()->SetScale(GetGameObject()->GetScale() * glm::vec3(0.5f, 1.0f, 0.5f));

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

void ComponentRendererSphere::Update(float deltaTime)
{

}

void ComponentRendererSphere::Render(sre::RenderPass &renderPass)
{
	renderPass.draw(
		_mesh,
		glm::rotate(
			GetGameObject()->transform,
			glm::pi<float>() * 0 / 2,
			glm::vec3(1, 0, 0)),
		_material
	);
}