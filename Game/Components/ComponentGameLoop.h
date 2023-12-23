#pragma once

#include "Engine/Component.h"
#include "ComponentGameLoop.h"
#include "../GameState.h"

class ComponentGameLoop : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;
	void Render(sre::RenderPass&) override;

public:
	void SetGameState(GameState new_state);
	GameState GetGameState();
	void OrderCompleted();

private:
	void ResetGame();
	void ClearDay();
	float time_until_next_order = 5;
	float initial_time_between_orders;
	float time_between_orders;
	float initial_customer_patience;
	float patience_decrease_pr_day;
	float customer_patience;
	float order_speedup_pr_day;
	int initial_orders_pr_day;
	int orders_pr_day;
	int orders_completed_today = 0;
	int orders_placed_today = 0;
	int day = 0;
	int table_spawn_day_interval;
	int increment_order_count_day_interval;
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
	std::string _death_reason = "";
};