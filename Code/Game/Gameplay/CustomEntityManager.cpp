#include "CustomEntityManager.h"
#include "./Fight/FightComponent.h"
#include "./Mobs/MobSpawnAreaComponent.h"
#include "Graphic/GraphicComponent.h"
#include "./Fight/SkillComponent.h"
#include "ECS/Basics/Physic/MovementComponent.h"
#include "ECS/Basics/Physic/PositionComponent.h"
#include "ECS/Basics/Physic/HitboxComponent.h"
#include "ECS/Basics/Physic/ForceComponent.h"
#include "ECS/Basics/Physic/CollidableComponent.h"
#include "Utils/SpritePath.h"
#include "./Fight/SkillsHolderComponent.h"
#include "Utils/NumberUtils.h"
#include "../Utils/IDs.h"

#include "Physic/BuildHitbox.h"
#include "ECS/Basics/Input/InputComponent.h"
#include "ECS/Basics/Physic/GravityAffectedComponent.h"

CustomEntityManager::CustomEntityManager(ska::GameEventDispatcher& ged) : 
	ska::PrefabEntityManager(ged) {
}

ska::EntityId CustomEntityManager::createSkill(const SkillsHolderComponent& shc, unsigned int index) {

	SkillComponent sc;
	//sc.noise = reader.getInt("Particle slope_noise");
	const auto& sd = shc.skills[index];
	sc.speed = sd.speed;
	sc.damage = -sd.buffEnemy.hp;
	sc.knockback = sd.knockback;
	sc.range = sd.range;
	const auto maxNoise = sd.noise;
	sc.noise = ska::NumberUtils::random(maxNoise/2, maxNoise);
	sc.amplitude = sd.amplitude;


	auto skill = createEntity();

	ska::PositionComponent pcSkill;
	ska::GraphicComponent gcSkill;
	gcSkill.desiredPriority = SKILLS_DEFAULT_DISPLAY_PRIORITY;
	gcSkill.animatedSprites.resize(1);
	gcSkill.animatedSprites[0].load(ska::SpritePath::getInstance().getPath(SPRITEBANK_SKILL, sd.id), 2, 2, 2);
	addComponent<ska::MovementComponent>(skill, ska::MovementComponent());
	addComponent<SkillComponent>(skill, std::move(sc));
	addComponent<ska::PositionComponent>(skill, std::move(pcSkill));
	ska::HitboxComponent hc;
	hc.xOffset = gcSkill.animatedSprites[0].getWidth() / 2 - 1;
	hc.yOffset = gcSkill.animatedSprites[0].getHeight() / 2 - 1;
	hc.width = gcSkill.animatedSprites[0].getWidth() / 2;
	hc.height = gcSkill.animatedSprites[0].getHeight() / 2;
	addComponent<ska::ForceComponent>(skill, ska::ForceComponent());
	addComponent<ska::HitboxComponent>(skill, std::move(hc));
	addComponent<ska::GraphicComponent>(skill, std::move(gcSkill));
	addComponent<ska::CollidableComponent>(skill, ska::CollidableComponent());
	return skill;
}

ska::EntityId CustomEntityManager::createTrainer(const ska::Point<int> startBlockPos, const unsigned int worldBlockSize) {
	auto trainer = PrefabEntityManager::createTrainer(startBlockPos, worldBlockSize);
	addComponent<MobSpawnAreaComponent>(trainer, MobSpawnAreaComponent());
	addComponent<FightComponent>(trainer, FightComponent());
	return trainer;
}

ska::EntityId CustomEntityManager::createCharacter(const ska::Point<int> startBlockPos, const int id, const unsigned int worldBlockSize, const std::string& name) {
	return PrefabEntityManager::createCharacter(startBlockPos, id, worldBlockSize, name);
}

void CustomEntityManager::fillCharacter(CustomEntityManager& em, ska::cp::Space& space, ska::EntityId character) {
	const auto& point = em.getComponent<ska::PositionComponent>(character);
	auto& hitbox = em.getComponent<ska::HitboxComponent>(character);
	const auto& gafc = em.getComponent<ska::GravityAffectedComponent>(character);
	const auto& fc = em.getComponent<ska::ForceComponent>(character);

	auto bc = ska::cp::BuildControlledRectangleHitbox(space, point, { hitbox.xOffset, hitbox.yOffset, static_cast<int>(hitbox.width), static_cast<int>(hitbox.height) }, fc.weight, character);
	ska::cp::AddTopDownConstraints(space, &space.getBody(bc.controlBodyIndex), space.getBody(bc.bodyIndex), gafc.friction * 50, gafc.rotationFriction * 30);

	em.addComponent(character, std::move(bc));
}

ska::EntityId CustomEntityManager::createTrainerNG(CustomEntityManager & em, ska::cp::Space& space, const ska::Point<int> startBlockPos, const unsigned int worldBlockSize, const std::string& name) {
	const auto trainer = em.createTrainer(startBlockPos, worldBlockSize);
	auto& hitbox = em.getComponent<ska::HitboxComponent>(trainer);
	
	fillCharacter(em, space, trainer);
	
	const auto& bc = em.getComponent<ska::cp::HitboxComponent>(trainer);
	const auto& shape = space.getShape(bc.shapeIndex);
	const auto& entityDimensions = shape.getDimensions();
	hitbox.xOffset = entityDimensions.x;
	hitbox.yOffset = entityDimensions.y - entityDimensions.h;
	hitbox.width = entityDimensions.w;
	hitbox.height = entityDimensions.h;

	return trainer;
}

ska::EntityId CustomEntityManager::createCharacterNG(CustomEntityManager& em, ska::cp::Space& space, const ska::Point<int> startBlockPos, const int id, const unsigned int worldBlockSize, const std::string& name) {
	const auto character = em.createCharacter(startBlockPos, id, worldBlockSize);
	const auto& point = em.getComponent<ska::PositionComponent>(character);
	auto& hitbox = em.getComponent<ska::HitboxComponent>(character);
	auto& gafc = em.getComponent<ska::GravityAffectedComponent>(character);
	auto& fc = em.getComponent<ska::ForceComponent>(character);
	gafc.friction = 10.F;
	fc.weight = 20.F;

	auto bc = ska::cp::BuildRectangleHitbox(space, point, { hitbox.xOffset, hitbox.yOffset, static_cast<int>(hitbox.width), static_cast<int>(hitbox.height) }, fc.weight, character);
	ska::cp::AddTopDownConstraints(space, nullptr, space.getBody(bc.bodyIndex), gafc.friction * 50, gafc.rotationFriction * 30);

	const auto& shape = space.getShape(bc.shapeIndex);
	const auto& entityDimensions = shape.getDimensions();
	hitbox.xOffset = entityDimensions.x;
	hitbox.yOffset = entityDimensions.y - entityDimensions.h;
	hitbox.width = entityDimensions.w;
	hitbox.height = entityDimensions.h;
	em.addComponent(character, std::move(bc));
	return character;
}
