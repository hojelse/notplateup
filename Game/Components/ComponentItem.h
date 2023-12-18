#pragma once
#include "ComponentInteractable.h"

class ComponentItem : public MyEngine::Component {
public:
	void Init(int type_id) {
		_type_id = type_id;
	}
	int GetTypeId() {
		return _type_id;
	}
private:
	int _type_id;
};
