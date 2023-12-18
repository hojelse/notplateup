#include "ComponentController.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Engine/MyEngine.h"

#include "SDL.h"
#include "ComponentEmitter.h"
#include <cmath>

void ComponentController::Init(rapidjson::Value &serializedData) {
	mov_speed = serializedData["movSpeed"].GetFloat();
	rot_speed = serializedData["rotSpeed"].GetFloat();

	auto go = GetGameObject().lock();
	if (!go) return;
	_body = go->FindComponent<ComponentPhysicsBody>();
}

void ComponentController::Update(float deltaTime) {
	auto delta_x = 0.0f;
	if (key_down_a) delta_x -= 1.0f;
	if (key_down_d) delta_x += 1.0f;
	auto delta_y = 0.0f;
	if (key_down_s) delta_y -= 1.0f;
	if (key_down_w) delta_y += 1.0f;
	auto delta_z = 0.0f;
	if (key_down_q) delta_z -= 1.0f;
	if (key_down_e) delta_z += 1.0f;

	auto delta_rot_horizontal = 0.0f;
	// if (key_down_j) delta_rot_horizontal += 1.0f;
	// if (key_down_l) delta_rot_horizontal -= 1.0f;
	auto delta_rot_vertical = 0.0f;
	// if (key_down_i) delta_rot_vertical -= 1.0f;
	// if (key_down_k) delta_rot_vertical += 1.0f;

	auto go = _body.lock();
	auto vec = glm::vec3(
			delta_x,
			delta_y,
			delta_z
	);
	auto normalized = glm::normalize(vec);
	if (!isnan(normalized.x)) direction_vector = normalized;
	float sign = direction_vector.x >= 0 ? -1.0 : 1.0;
	auto newAngle = acos(glm::dot(direction_vector, glm::vec3(0, -1, 0))) * sign;
	if (!std::isnan(newAngle)) direction_angle = newAngle;
	go->setLinearVelocity(
			vec * deltaTime * mov_speed
	);

	// go->transform = glm::translate(
	// 	glm::rotate(
	// 		glm::rotate(
	// 			go->transform,
	// 			delta_rot_horizontal * deltaTime * rot_speed,
	// 			glm::vec3(0, 1, 0)
	// 		),
	// 		delta_rot_vertical * deltaTime * rot_speed,
	// 		glm::vec3(1, 0, 0)
	// 	),
	// 	glm::vec3(
	// 		delta_x * deltaTime * mov_speed,
	// 		delta_y * deltaTime * mov_speed,
	// 		delta_z * deltaTime * mov_speed
	// 	)
	// );

}

void ComponentController::KeyEvent(SDL_Event &event) {
	switch (event.key.keysym.sym) {
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
		case SDLK_q:
			key_down_q = event.type == SDL_KEYDOWN;
			break;
		case SDLK_e:
			key_down_e = event.type == SDL_KEYDOWN;
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
		case SDLK_o:
			key_down_o = event.type == SDL_KEYDOWN;
			if (key_down_o) Interact();
			break;
		case SDLK_p:
			key_down_p = event.type == SDL_KEYDOWN;
			break;
	}
}

void ComponentController::Interact() {
	auto go = GetGameObject().lock();
	auto pos = go->GetPosition();
	auto rotation = go->GetRotation();
	glm::vec3 euler = glm::eulerAngles(rotation);

	int x = std::floor(pos.x + direction_vector.x + 0.5f);
	int y = std::floor(pos.y + direction_vector.y + 0.5f);

	auto name = "box-" + std::to_string(x) + "-" + std::to_string(y);

	auto engine = MyEngine::Engine::GetInstance();
	auto box = engine->GetGameObject(name).lock();
	auto held = engine->GetGameObject("box-held").lock();
	auto emitter = box->FindComponent<ComponentEmitter>().lock();
	if (emitter) {
		std::cout << "emitter!!" << std::endl;
		emitter->Interact();
	}
//	if (box && !held) {
//		std::cout << "pick up!" << std::endl;
//		box->SetPosition(
//			glm::vec3(-2, -2, 0)
//		);
//		box->SetName("box-held");
//		engine->_gameObjects.erase(name);
//		engine->_gameObjects["box-held"] = box;
//
//		action_timeout = 0.2f;
//	} else if (!box && held) {
//		std::cout << "place!" << std::endl;
//		held->SetPosition(
//			glm::vec3(x, y, 0)
//		);
//		held->SetName(name);
//		engine->_gameObjects.erase("box-held");
//		engine->_gameObjects[name] = held;
//		action_timeout = 0.2f;
//	} else {
//        std::cout << "idk bruv " << (box != nullptr) << " " << (held != nullptr) << std::endl;
//    }
}

float ComponentController::GetAngle() {
	return direction_angle;
}