#include "ComponentController.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "SDL.h"

void ComponentController::Init(rapidjson::Value& serializedData) {
	mov_speed = serializedData["movSpeed"].GetFloat();
	rot_speed = serializedData["rotSpeed"].GetFloat();
}

void ComponentController::Update(float deltaTime) {
	auto delta_x = 0.0f;
	if (key_down_a) delta_x -= 1.0f;
	if (key_down_d) delta_x += 1.0f;
	auto delta_y = 0.0f;
	// if (key_down_q) delta_y -= 1.0f;
	// if (key_down_e) delta_y += 1.0f;
	auto delta_z = 0.0f;
	if (key_down_w) delta_z -= 1.0f;
	if (key_down_s) delta_z += 1.0f;

	auto delta_rot_horizontal = 0.0f;
	if (key_down_j) delta_rot_horizontal += 1.0f;
	if (key_down_l) delta_rot_horizontal -= 1.0f;
	auto delta_rot_vertical = 0.0f;
	// if (key_down_i) delta_rot_vertical -= 1.0f;
	// if (key_down_k) delta_rot_vertical += 1.0f;

	auto go = GetGameObject().lock();
	auto transform = go->transform;
	transform = glm::translate(
		glm::rotate(
			glm::rotate(
				transform,
				delta_rot_horizontal * deltaTime * rot_speed,
				glm::vec3(0, 1, 0)
			),
			delta_rot_vertical * deltaTime * rot_speed,
			glm::vec3(1, 0, 0)
		),
		glm::vec3(delta_x * deltaTime * mov_speed, delta_y * deltaTime * mov_speed, delta_z * deltaTime * mov_speed)
	);
}

void ComponentController::KeyEvent(SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
		case SDLK_w:
			key_down_w = event.type == SDL_KEYDOWN;
			break;
		case SDLK_a:
			key_down_a = event.type == SDL_KEYDOWN;
			break;
		case SDLK_s:
			key_down_s = event.type == SDL_KEYDOWN;
			break;
		case SDLK_d:
			key_down_d = event.type == SDL_KEYDOWN;
			break;
		case SDLK_i:
			key_down_i = event.type == SDL_KEYDOWN;
			break;
		case SDLK_j:
			key_down_j = event.type == SDL_KEYDOWN;
			break;
		case SDLK_k:
			key_down_k = event.type == SDL_KEYDOWN;
			break;
		case SDLK_l:
			key_down_l = event.type == SDL_KEYDOWN;
			break;
		case SDLK_q:
			key_down_q = event.type == SDL_KEYDOWN;
			break;
		case SDLK_e:
			key_down_e = event.type == SDL_KEYDOWN;
			break;
	}
}

void ComponentController::Render(sre::RenderPass&) {
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