#include "ScriptCommandsSystem.h"
#include "../Commands/CommandAddPokemon.h"
#include "../Commands/CommandChoice.h"
#include "../Commands/CommandCinematic.h"
#include "../Commands/CommandDirection.h"
#include "../Commands/CommandFollow.h"
#include "../Commands/CommandHeal.h"
#include "../Commands/CommandHideCharacter.h"
#include "../Commands/CommandHideGUI.h"
#include "../Commands/CommandKillEntity.h"
#include "../Commands/CommandMessage.h"
#include "../Commands/CommandMove.h"
#include "../Commands/CommandPlayAnimation.h"
#include "../Commands/CommandPlayCrumbling.h"
#include "../Commands/CommandPlayerPresence.h"
#include "../Commands/CommandPlayShaking.h"
#include "../Commands/CommandPokemonOut.h"
#include "../Commands/CommandShop.h"
#include "../Commands/CommandStop.h"
#include "../Commands/CommandStuckCharacter.h"
#include "../Commands/CommandTeleport.h"
#include "../Commands/CommandThrowPokebal.h"
#include "../Commands/CommandTranslationCamera.h"

using namespace std;

ScriptCommandsSystem::ScriptCommandsSystem(ska::EntityManager& entityManager, ska::Savegame& saveGame) : ScriptBasicCommandsSystem(ScriptCommandHelper(entityManager), entityManager, saveGame) {
}

void ScriptCommandsSystem::ScriptCommandHelper::setupCommands(std::unordered_map<std::string, ska::CommandPtr>& c) const {
	BasicScriptCommandHelper::setupCommands(c);
	c["move"] = move(ska::CommandPtr(new CommandMove(m_entityManager)));
	c["message"] = move(ska::CommandPtr(new CommandMessage(m_entityManager)));
	c["choice"] = move(ska::CommandPtr(new CommandChoice(m_entityManager)));
	c["heal"] = move(ska::CommandPtr(new CommandHeal(m_entityManager)));
	c["stop"] = move(ska::CommandPtr(new CommandStop(m_entityManager)));
	c["direction"] = move(ska::CommandPtr(new CommandDirection(m_entityManager)));
	c["animation"] = move(ska::CommandPtr(new CommandPlayAnimation(m_entityManager)));
	c["crumbling"] = move(ska::CommandPtr(new CommandPlayCrumbling(m_entityManager)));
	c["shaking"] = move(ska::CommandPtr(new CommandPlayShaking(m_entityManager)));
	c["camera_translation"] = move(ska::CommandPtr(new CommandTranslationCamera(m_entityManager)));
	c["stuck_char"] = move(ska::CommandPtr(new CommandStuckCharacter(m_entityManager)));
	c["hide_char"] = move(ska::CommandPtr(new CommandHideCharacter(m_entityManager)));
	c["follow_char"] = move(ska::CommandPtr(new CommandFollow(m_entityManager)));
	c["shop"] = move(ska::CommandPtr(new CommandShop(m_entityManager)));
	c["hide_gui"] = move(ska::CommandPtr(new CommandHideGUI(m_entityManager)));
	c["teleport"] = move(ska::CommandPtr(new CommandTeleport(m_entityManager)));
	c["cinematic"] = move(ska::CommandPtr(new CommandCinematic(m_entityManager)));
	c["pokeball"] = move(ska::CommandPtr(new CommandThrowPokebal(m_entityManager)));
	c["kill"] = move(ska::CommandPtr(new CommandKillEntity(m_entityManager)));
	c["add_pokemon"] = move(ska::CommandPtr(new CommandAddPokemon(m_entityManager)));
	c["throw_pokemon"] = move(ska::CommandPtr(new CommandPokemonOut(m_entityManager)));
	c["player_presence"] = move(ska::CommandPtr(new CommandPlayerPresence(m_entityManager)));
}


ScriptCommandsSystem::~ScriptCommandsSystem()
{
}