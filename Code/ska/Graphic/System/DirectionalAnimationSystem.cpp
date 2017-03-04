#include "DirectionalAnimationSystem.h"
#include "../../Utils/RectangleUtils.h"

ska::DirectionalAnimationSystem::DirectionalAnimationSystem(EntityManager& entityManager) : System(entityManager) {
}

void ska::DirectionalAnimationSystem::refresh() {
	for (EntityId entityId : m_processed) {
		GraphicComponent& gc = m_entityManager.getComponent<GraphicComponent>(entityId);
		DirectionalAnimationComponent& dac = m_entityManager.getComponent<DirectionalAnimationComponent>(entityId);
		MovementComponent& mov = m_entityManager.getComponent<MovementComponent>(entityId);

		if (gc.sprite.empty()) {
			break;
		}

		//ska::Rectangle base de l'animation
		AnimatedTexture& texture = gc.sprite[0];
		Rectangle spritePos = texture.getOffsetBase();
		const int spriteHeight = texture.getHeight();
		

		if (((int)mov.vx) == 0 && ((int)mov.vy) == 0) {
			texture.stop(true);
			texture.reset();
		} else {
			texture.stop(false);
		}

		const int xMove = (int)(mov.vx + mov.ax + 0.5);
		const int yMove = (int)(mov.vy + mov.ay + 0.5);
		if (xMove != 0 || yMove != 0) {
			dac.direction = RectangleUtils::getDirectionFromPos(Point<int>(0, 0), Point<int>(xMove, yMove));
		}

		switch (dac.direction)
		{
		case 0:
			spritePos.y = spriteHeight * 2;
			break;
		case 1:
			spritePos.y = spriteHeight;
			break;
		case 2:
			spritePos.y = 0;
			break;
		case 3:
			spritePos.y = spriteHeight * 3;
			break;
		case 4:
			spritePos.y = spriteHeight * 7;
			break;
		case 5:
			spritePos.y = spriteHeight * 5;
			break;
		case 6:
			spritePos.y = spriteHeight * 4;
			break;
		case 7:
			spritePos.y = spriteHeight * 6;
			break;
		default:
			break;
		}

		texture.setOffset(spritePos);
	}
}

ska::DirectionalAnimationSystem::~DirectionalAnimationSystem() {
}
