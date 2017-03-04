#pragma once
#include <unordered_set>
#include "../../ska/ECS/Component.h"

namespace ska {
	class CollidableComponent : public Component {
	public:
		CollidableComponent() = default;
		~CollidableComponent() = default;
		std::unordered_set<int> authorizedBlockIds;
	};
}