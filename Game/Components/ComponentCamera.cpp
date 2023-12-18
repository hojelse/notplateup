#include "ComponentCamera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "rapidjson/document.h"
#include "Engine/MyEngine.h"

void ComponentCamera::Init(rapidjson::Value& serializedData) {
	_camera = MyEngine::Engine::GetInstance()->GetCamera();

	auto& projectionParams = serializedData["projectionParams"];
	// this is EXTREMELY unsafe. It would be fine if we both
	//   - ensure that the json is always read and write by code, and never touched by humans
	//   - hide all of this where client programmers don't need to deal with it
	// which is not our case. But for the sake of simplicity, we will accept it
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

	// we don't really care about these for gameObjects, but we need to matck glm::decompose() signature
	glm::vec3 skew; glm::vec4 perspective;

	glm::decompose(go->transform, scale, rotation, position, skew, perspective);

	_camera->setPositionAndRotation(position, glm::degrees(glm::eulerAngles(rotation)));
}

void ComponentCamera::Render(sre::RenderPass&) {
    auto gameObject = GetGameObject().lock();
    glm::vec3 position = gameObject->GetPosition();
    glm::vec3 rotation = gameObject->GetEulerAngles();
    glm::vec3 scale = gameObject->GetScale();

    ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 75), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    if (ImGui::DragFloat3("Position", &(position.x)))
        gameObject->SetPosition(position);
    if (ImGui::DragFloat3("Rotation", &(rotation.x)))
        gameObject->SetEulerAngles(rotation);
    if (ImGui::DragFloat3("Scale", &(scale.x)))
        gameObject->SetScale(scale);
    ImGui::End();
}