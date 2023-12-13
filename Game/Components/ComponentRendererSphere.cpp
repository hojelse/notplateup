#include "ComponentRendererSphere.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <unistd.h>
#include <cmath>

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

	auto go = GetGameObject().lock();
	prev_pos = go->GetPosition();
}

void ComponentRendererSphere::Update(float deltaTime)
{
	auto go = GetGameObject().lock();
	auto curr_pos = go->GetPosition();

	// movement vector
	auto dir_vec = glm::normalize(curr_pos - prev_pos);
	// printf("dir_vec: %f, %f, %f\n", dir_vec.x, dir_vec.y, dir_vec.z);

	// rotation angle
	auto sign = dir_vec.x >= 0 ? -1 : 1;
	auto angle = acos(glm::dot(dir_vec, glm::vec3(0, 0, -1))) * sign;

	// handle nan
	_angle = std::isnan(angle) ? 0 : angle;

	prev_pos = curr_pos;
}

void ComponentRendererSphere::Render(sre::RenderPass &renderPass)
{
	auto go = GetGameObject().lock();
	renderPass.draw(
		_mesh,
		glm::rotate(
			go->transform,
			_angle,
			glm::vec3(0, 1, 0)),
		_material
	);
}