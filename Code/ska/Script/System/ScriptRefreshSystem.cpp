#include "ScriptRefreshSystem.h"
#include "../ScriptComponent.h"
#include "../../Utils/SkaConstants.h"
#include "../../Utils/StringUtils.h"
#include "../../Utils/FileUtils.h"
#include "../../ECS/EntityManager.h"
#include "../../Exceptions/InvalidPathException.h"
#include "../ScriptTriggerType.h"

using namespace std;


ska::ScriptRefreshSystem::ScriptRefreshSystem(ScriptAutoSystem& scriptAutoSystem, const InputContextManager& icm, const unsigned int wBlockSize, EntityManager& entityManager) :
System<std::unordered_set<EntityId>, PositionComponent, ScriptAwareComponent>(entityManager), 
ScriptPositionSystemAccess(entityManager),
m_icm(icm), m_blockSize(wBlockSize), m_scriptAutoSystem(scriptAutoSystem) {

}

void ska::ScriptRefreshSystem::refresh() {
	const InputActionContainer& iac = m_icm.getActions();
	EntityManager& entityManager = ScriptPositionSystemAccess::m_entityManager;
	std::vector<EntityId> toDelete;

	for (EntityId entityId : System<std::unordered_set<EntityId>, PositionComponent, ScriptAwareComponent>::m_processed) {
		ScriptAwareComponent& sac = entityManager.getComponent<ScriptAwareComponent>(entityId);
		const PositionComponent& pc = entityManager.getComponent<PositionComponent>(entityId);

		for (EntityId targets : ScriptPositionSystemAccess::m_processed) {
			ScriptSleepComponent& scriptData = entityManager.getComponent<ScriptSleepComponent>(targets);
			
			EntityId scriptEntity;
			
			switch (scriptData.triggeringType) {
			case EnumScriptTriggerType::AUTO:
				startScript(targets, entityId);
				toDelete.push_back(targets);
				break;

			case EnumScriptTriggerType::ACTION:
				if (!iac[InputAction::DoAction]) {
					break;
				}

			case EnumScriptTriggerType::MOVE:
				scriptEntity = findNearScriptComponentEntity(entityManager, pc, targets);
				if (scriptEntity != UINT_MAX) {
					startScript(scriptEntity, entityId);
				}
				break;
			default:
				break;
			}


		}
	}

	for (EntityId targets : toDelete) {
		entityManager.removeComponent<ScriptSleepComponent>(targets);
	}

	m_scriptAutoSystem.update();

}

void ska::ScriptRefreshSystem::update() {
	System<std::unordered_set<EntityId>, PositionComponent, ScriptAwareComponent>::update();
}

void ska::ScriptRefreshSystem::registerNamedScriptedEntity(const std::string& nameEntity, const EntityId entity) {
	m_scriptAutoSystem.registerNamedScriptedEntity(nameEntity, entity);
}

void ska::ScriptRefreshSystem::clearNamedScriptedEntities() {
	m_scriptAutoSystem.clearNamedScriptedEntities();
}

void ska::ScriptRefreshSystem::startScript(const EntityId scriptEntity, const EntityId origin) {
	m_scriptAutoSystem.registerScript(NULL, scriptEntity, origin);
}

const ska::EntityId ska::ScriptRefreshSystem::findNearScriptComponentEntity(EntityManager& entityManager, const PositionComponent& entityPos, EntityId script) const {
	const unsigned int blockSizeSquared = m_blockSize * m_blockSize;
	
	ScriptSleepComponent& scriptData = entityManager.getComponent<ScriptSleepComponent>(script);
	PositionComponent& scriptPos = entityManager.getComponent<PositionComponent>(script);

	int varX = (scriptPos.x - entityPos.x);
	int varY = (scriptPos.y - entityPos.y);
	int varZ = (scriptPos.z - entityPos.z);
	unsigned int distanceSquared = varX * varX + varY * varY + varZ * varZ;
	if (distanceSquared < blockSizeSquared) {
		return script;
	}
	
	return UINT_MAX;
}

ska::ScriptRefreshSystem::~ScriptRefreshSystem() {
}
