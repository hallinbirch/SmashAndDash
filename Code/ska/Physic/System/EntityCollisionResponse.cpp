#include "EntityCollisionResponse.h"
#include "../../Physic/ForceComponent.h"
#include "../../Physic/MovementComponent.h"
#include "../../ECS/EntityManager.h"
#include "CollisionSystem.h"
#include "../../ECS/EntityManager.h"
#include "../CollisionComponent.h"

ska::EntityCollisionResponse::EntityCollisionResponse(GameEventDispatcher& ged, EntityManager& em) :
	EntityCollisionObserver(bind(&EntityCollisionResponse::onEntityCollision, this, std::placeholders::_1)),
	m_entityManager(em), 
	m_ged(ged) {
	m_ged.Observable<CollisionEvent>::addObserver(*this);
}

ska::EntityCollisionResponse::EntityCollisionResponse(std::function<bool(CollisionEvent&)> onEntityCollision, GameEventDispatcher& ged, EntityManager& em) :
EntityCollisionObserver(onEntityCollision),
m_entityManager(em),
m_ged(ged) {
	m_ged.Observable<CollisionEvent>::addObserver(*this);
}

bool ska::EntityCollisionResponse::onEntityCollision(CollisionEvent& e) {
	if (e.collisionComponent == nullptr) {
		return false;
	}

	auto col = *e.collisionComponent;
	ForceComponent& ftarget = m_entityManager.getComponent<ForceComponent>(col.target);
	//MovementComponent& mtarget = m_entityManager.getComponent<MovementComponent>(col.target);

	ForceComponent& forigin = m_entityManager.getComponent<ForceComponent>(col.origin);
	MovementComponent& moveComponent = m_entityManager.getComponent<MovementComponent>(col.origin);

	if (col.xaxis) {
		ftarget.x += (moveComponent.vx + moveComponent.ax)*ftarget.weight;
	}

	if (col.yaxis) {
		ftarget.y += (moveComponent.vy + moveComponent.ay)*ftarget.weight;
	}

	if (col.xaxis) {
		forigin.x = -ftarget.x / 2;
	}

	if (col.yaxis) {
		forigin.y = -ftarget.y / 2;
	}
	return true;
}

ska::EntityCollisionResponse::~EntityCollisionResponse() {
	m_ged.Observable<CollisionEvent>::removeObserver(*this);
}