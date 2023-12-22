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
			std::string texture_id = serializedData["layout"][y][x].GetString();
			if (texture_id.empty()) continue;
			CreateBox(texture_id, x, y);
		}
	}
}

void ComponentLevelLayout::CreateBox(std::string texture_id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto name = "box-" + std::to_string(x) + "-" + std::to_string(y);
	auto go = engine->CreateGameObject(name).lock();

	if (texture_id.find("box-") != std::string::npos) {
		auto emitter = std::make_shared<ComponentEmitter>();
		emitter->Init(texture_id);
		go->AddComponent(emitter);
	} else if (texture_id.find("consumer") != std::string::npos) {
		auto consumer = std::make_shared<ComponentConsumer>();
		consumer->Init(texture_id);
		go->AddComponent(consumer);
	} else if (texture_id.find("box") != std::string::npos) {
		auto table = std::make_shared<ComponentTable>();
		table->Init(texture_id);
		go->AddComponent(table);
	} else {
		// don't
		std::cout << "this should never happen :-) you messed up boy" << std::endl;
		return;
	}

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

void ComponentLevelLayout::Update(float deltaTime) {

}

void ComponentLevelLayout::Render(sre::RenderPass& renderPass) {

}