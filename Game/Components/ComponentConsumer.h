#pragma once
#include "ComponentInteractable.h"

class ComponentConsumer : public ComponentInteractable {
public:
	void Init(rapidjson::Value&) override;
	void Init(int) override;
private:
	void Interact() override;
	void CreateItemIndicator();
};
