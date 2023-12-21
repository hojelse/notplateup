#pragma once
#include "ComponentInteractable.h"

class ComponentConsumer : public ComponentInteractable {
public:
	void Init(int) override;
	void Update(float) override;
	bool is_ordering = false;
	float patience_left =  10;
	float total_patience;
	void CreateOrder(int item_id, float patience);
private:
	void Interact() override;
	void CreateConsumerIndicator(int id, int x, int y);
	std::shared_ptr<MyEngine::GameObject> indicator;
};
