#include "Engine/Component.h"
#include "Engine/MyEngine.h"

class ComponentEmitter : public MyEngine::Component {
public:
	void Init(int id);

	void Interact();

private:
	int _id;
};
