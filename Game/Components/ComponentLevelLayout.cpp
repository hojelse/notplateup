#include "ComponentLevelLayout.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/MyEngine.h"
#include "Engine/GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentEmitter.h"
#include "ComponentConsumer.h"
#include "ComponentTable.h"
#include "ComponentRendererSquare.h"
#include "ComponentFollowTarget.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentLevelLayout::Init(rapidjson::Value& serializedData) {
	auto dimy = serializedData["layout"].Size();
	auto dimx = serializedData["layout"][0].Size();
	
	for (int y = 0; y < dimy; y++) {
		for (int x = 0; x < dimx; x++) {
			auto texture_id = serializedData["layout"][y][x].GetInt();

			if (texture_id < 0) continue;
			if (texture_id > 6*16-1) continue;
			CreateBox(texture_id, x, y);
		}
	}
}

void ComponentLevelLayout::CreateBox(int texture_id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto name = "box-" + std::to_string(x) + "-" + std::to_string(y);
	auto go = engine->CreateGameObject(name).lock();

	auto r0 = std::make_shared<ComponentRendererSquare>();
	r0->Init(texture_id);
	r0->SetRotation(true, false, 3);
	go->AddComponent(r0);

	auto r1 = std::make_shared<ComponentRendererSquare>();
	r1->Init(texture_id);
	r1->SetRotation(true, false, 1);
	go->AddComponent(r1);

	for (int i = 0; i < 4; i++) {
		auto r2 = std::make_shared<ComponentRendererSquare>();
		r2->Init(texture_id);
		r2->SetRotation(false, true, i);
		go->AddComponent(r2);
	}

	switch (texture_id)
	{
		case 6:
		case 8: {
				auto emitter = std::make_shared<ComponentEmitter>();
				emitter->Init(texture_id);
				go->AddComponent(emitter);
				break;
			}
		case 7:
		case 9: {
			auto consumer = std::make_shared<ComponentConsumer>();
			consumer->Init(texture_id);
			go->AddComponent(consumer);

			CreateConsumerIndicator(texture_id, x, y, name);
			break;
		}
		default: {
			auto table = std::make_shared<ComponentTable>();
				table->Init(texture_id);
				go->AddComponent(table);
			break;
		}
	}

	auto pos = glm::vec3(x, y, 0);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(1, 1, 1);

	go->transform =
			glm::translate(pos) *
			glm::mat4_cast(glm::quat(glm::radians(rot))) *
			glm::scale(scl);

	auto body = go->CreateComponent<ComponentPhysicsBody>().lock();
	auto bodyType = b2_staticBody;
	auto isSensor = false;
	glm::vec2 size { 0.5, 0.5 };
	body->CreateBody(bodyType, isSensor, size);
}

void ComponentLevelLayout::CreateConsumerIndicator(int id, int x, int y, std::string follow_target) {
	auto engine = MyEngine::Engine::GetInstance();
	auto name = "indicator-" + std::to_string(x) + "-" + std::to_string(y);
	auto item = engine->CreateGameObject(name).lock();

	auto texture_name = "carrot";
	switch (id)
	{
		case 7: {
			texture_name = "tomato";
			break;
		}
		case 9: {
			texture_name = "carrot";
			break;
		}
		default: {
			texture_name = "tomato";
			break;
		}
	}

	auto r2 = std::make_shared<ComponentRendererSquare>();
	r2->Init(texture_name);
	r2->SetRotation(false, true, 0);
	item->AddComponent(r2);

	auto pos = glm::vec3(x, y, 2);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(0.5, 0.5, 0.5);

	item->transform =
		glm::translate(pos) *
		glm::mat4_cast(glm::quat(glm::radians(rot))) *
		glm::scale(scl);
}

void ComponentLevelLayout::Update(float deltaTime) {

}

void ComponentLevelLayout::Render(sre::RenderPass& renderPass) {

}