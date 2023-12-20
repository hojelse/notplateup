#pragma once

#include "Engine/Component.h"
#include "ComponentGameLoop.h"

class ComponentGameLoop : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;

public:
	void SubmitItem(int idx);

private:
	bool key_down_i = false;
	float _patience;
	float _elapsed;
	int _n;
	std::vector<int> _item_states;
	int _game_state;
	std::vector<float> _start;
	std::vector<int> _item_id;
};