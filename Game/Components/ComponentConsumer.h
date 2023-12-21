#pragma once
#include "ComponentInteractable.h"

class ComponentConsumer : public ComponentInteractable {
public:
	void Init(int) override;
	void Update(float) override;
	bool is_ordering = false;
	float patience_left =  10;
	void CreateOrder(int item_id);
private:
	void Interact() override;
	void CreateConsumerIndicator(int id, int x, int y);
};
