#pragma once
#include <unordered_set>
#include "../GraphicComponent.h"
#include "../DirectionalAnimationComponent.h"
#include "../../Physic/MovementComponent.h"
#include "../../ECS/System.h"

namespace ska {
	class DirectionalAnimationSystem : public System<std::unordered_set<EntityId>, GraphicComponent, MovementComponent, DirectionalAnimationComponent> {
	public:
		explicit DirectionalAnimationSystem(EntityManager& entityManager);
		DirectionalAnimationSystem& operator=(const DirectionalAnimationSystem&) = delete;
		virtual ~DirectionalAnimationSystem();
		virtual void refresh() override;

	};
}

