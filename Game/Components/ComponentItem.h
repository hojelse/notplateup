#pragma once
#include "ComponentInteractable.h"

class ComponentItem : public MyEngine::Component {
public:
	void Init(std::string type_id) {
		_type_id = type_id;
	}
	std::string GetTypeId() {
		return _type_id;
	}
private:
	std::string _type_id;
};
