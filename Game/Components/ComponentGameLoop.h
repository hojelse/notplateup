#pragma once

#include "Engine/Component.h"
#include "ComponentGameLoop.h"
#include "../GameState.h"

class ComponentGameLoop : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;

public:
	void SetGameState(GameState new_state);
	GameState GetGameState();
	void ResetGame();
	void ClearDay();
	void OrderCompleted();

private:
	float time_until_next_order = 5;
	float initial_time_between_orders;
	float time_between_orders;
	float initial_customer_patience;
	float customer_patience;
	int initial_orders_pr_day;
	int orders_pr_day;
	int orders_completed_today = 0;
	std::vector<std::string> items;
	bool queue = false;
	bool key_down_i = false;
	float _patience = 10;
	GameState _game_state = EDIT;
	float _elapsed;
	int _n;
	std::vector<int> _item_states;
	std::vector<float> _start;
	std::vector<int> _item_id;
};