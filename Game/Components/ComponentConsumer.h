#pragma once
#include "ComponentInteractable.h"

class ComponentConsumer : public ComponentInteractable {
public:
	void Init(std::string) override;
	void Update(float) override;
	bool is_ordering = false;
	float patience_left =  10;
	float total_patience;
	void CreateOrder(std::string item_id, float patience);
	std::shared_ptr<MyEngine::GameObject> indicator;
private:
	void Interact() override;
	void CreateConsumerIndicator(std::string id, int x, int y);
};
