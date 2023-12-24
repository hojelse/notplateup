#include "ComponentCamera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "rapidjson/document.h"
#include "Engine/MyEngine.h"

void ComponentCamera::Init(rapidjson::Value& serializedData) {
	_camera = MyEngine::Engine::GetInstance()->GetCamera();

	auto& projectionParams = serializedData["projectionParams"];
	float p_0 = projectionParams[0].GetFloat();
	float p_1 = projectionParams[1].GetFloat();
	float p_2 = projectionParams[2].GetFloat();

	switch (serializedData["projectionType"].GetInt()) {
		case ProjectionType::Orthographic: _camera->setOrthographicProjection(p_0, p_1, p_2); break;
		case ProjectionType::Perspective:  _camera->setPerspectiveProjection(p_0, p_1, p_2);  break;
	}

	_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3{ 0,1,0 });
}

void ComponentCamera::Update(float) {
	auto go = GetGameObject().lock();
	glm::vec3 position, scale;
	glm::quat rotation;

	glm::vec3 skew; glm::vec4 perspective;

	glm::decompose(go->transform, scale, rotation, position, skew, perspective);

	_camera->setPositionAndRotation(position, glm::degrees(glm::eulerAngles(rotation)));
}
