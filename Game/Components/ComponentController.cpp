#include "ComponentController.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Engine/MyEngine.h"

#include "SDL.h"
#include "ComponentInteractable.h"
#include "ComponentFollowTarget.h"
#include "ComponentGameLoop.h"
#include "ComponentRendererSquare.h"
#include "ComponentNotifier.h"
#include <cmath>

void ComponentController::Init(rapidjson::Value &serializedData) {
	mov_speed = serializedData["movSpeed"].GetFloat();
	auto engine = MyEngine::Engine::GetInstance();
	auto game_loop = engine->GetGameObject("game_loop").lock()->FindComponent<ComponentGameLoop>().lock();
	_game_loop = game_loop;
	auto go = GetGameObject().lock();
	if (!go) return;
	_body = go->FindComponent<ComponentPhysicsBody>();
	_body.lock()->setCircleShape(false);
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
	if (key_down_shift) {
		go->setLinearVelocity({0, 0});
	} else {
		go->setLinearVelocity(
				vec * deltaTime * mov_speed
		);
	}

	if (dash_cooldown > 0) dash_cooldown -= deltaTime;
	if (key_down_k && dash_cooldown <= 0) {
		dash_timer = 0.2;
		dash_cooldown = 1;
	}
	if (dash_timer >= 0) {
		go->addImpulse(direction_vector * deltaTime * 30.0f);
		dash_timer -= deltaTime;
	}
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
		case SDLK_k:
			key_down_k = event.type == SDL_KEYDOWN;
			break;
		case SDLK_o:
			key_down_o = event.type == SDL_KEYDOWN;
			if (key_down_o) {
				switch (_game_loop->GetGameState()) {
					case PLAYING:
						Interact();
						break;
					case EDIT:
						MoveTable();
						break;
					default:
						break;
				}
			}
			break;
		case SDLK_LSHIFT:
			key_down_shift = event.type == SDL_KEYDOWN;
			break;
	}
}

void ComponentController::MoveTable() {
	auto go = GetGameObject().lock();
	auto pos = go->GetPosition();
	auto rotation = go->GetRotation();

	int x = std::floor(pos.x + direction_vector.x + 0.5f);
	int y = std::floor(pos.y + direction_vector.y + 0.5f);

	auto box_id = "box-" + std::to_string(x) + "-" + std::to_string(y);

	auto engine = MyEngine::Engine::GetInstance();
	auto box_exists = engine->GameObjectExists(box_id);
	auto box_held = engine->GameObjectExists("box-held");
	auto notifier = engine->GetGameObject("notifier").lock()->FindComponent<ComponentNotifier>().lock();
	if (box_exists && !box_held) {
		std::cout << "pickup box!" << std::endl;
		auto box = engine->GetGameObject(box_id).lock();
		box->SetPosition({-1, -1, 0});
		engine->RenameGameObject(box_id, "box-held");

		// Make box follow player
		auto f = std::make_shared<ComponentFollowTarget>();
		f->Init(
			"player",
			glm::vec3{0, 0, 2}
		);
		box->AddComponent(f);

		// remove collider
		auto body = box->FindComponent<ComponentPhysicsBody>().lock();
		box->RemoveComponent(body);

	} else if (!box_exists && box_held) {
		auto ground_tile = engine->GetGameObject("floor-" + std::to_string(x) + "-" + std::to_string(y)).lock();
		if (ground_tile) {
			bool is_floor = ground_tile->FindComponent<ComponentRendererSquare>().lock()->is_floor;
			if (!is_floor) {
				notifier->SetMessage("Boxes can only be placed on floor");
				std::cout << "boxes can only be placed on floor!" << std::endl;
				return;
			}
		}
		std::cout << "place box!" << std::endl;
		auto box = engine->GetGameObject("box-held").lock();
		box->SetPosition(
			glm::vec3(x, y, 0)
		);
		engine->RenameGameObject("box-held", box_id);

		// Make box NOT follow player
		auto f = box->FindComponent<ComponentFollowTarget>().lock();
		box->RemoveComponent(f);

		// add collider
		auto body = box->CreateComponent<ComponentPhysicsBody>().lock();
		auto bodyType = b2_staticBody;
		auto isSensor = false;
		glm::vec2 size { 0.5, 0.5 };
		body->CreateBody(bodyType, isSensor, size);
		box->AddComponent(body);
	} else {
		std::cout << "idk bruv " << std::endl;
	}
}

void ComponentController::Interact() {
	auto go = GetGameObject().lock();
	auto pos = go->GetPosition();
	auto rotation = go->GetRotation();

	int x = std::floor(pos.x + direction_vector.x + 0.5f);
	int y = std::floor(pos.y + direction_vector.y + 0.5f);

	auto engine = MyEngine::Engine::GetInstance();

	auto box_id = "box-" + std::to_string(x) + "-" + std::to_string(y);

	if (engine->GameObjectExists(box_id)) {
		auto box = engine->GetGameObject(box_id).lock();

		auto interactable = box->FindComponent<ComponentInteractable>().lock();
		if (interactable) {
			interactable->Interact();
		}
	}

}

float ComponentController::GetAngle() {
	return direction_angle;
}