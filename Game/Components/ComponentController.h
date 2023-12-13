#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentPhysicsBody.h"


class ComponentController : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;
	void Render(sre::RenderPass&) override;

private:
	void Interact();
	float mov_speed;
	float rot_speed;
	std::weak_ptr<ComponentPhysicsBody> _body;

	bool key_down_w = false;
	bool key_down_a = false;
	bool key_down_s = false;
	bool key_down_d = false;
	bool key_down_q = false;
	bool key_down_e = false;

	bool key_down_j = false;
	bool key_down_k = false;
	bool key_down_i = false;
	bool key_down_l = false;
	bool key_down_o = false;
	bool key_down_p = false;

	float action_timeout = 0.0f;
};