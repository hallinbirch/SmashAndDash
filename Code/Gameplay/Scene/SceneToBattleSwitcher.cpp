#include "SceneToBattleSwitcher.h"
#include "SceneFight.h"
#include "../World/WorldScene.h"
#include "../../ska/Scene/SceneHolder.h"
#include "../Fight/FightComponent.h"

SceneToBattleSwitcher::SceneToBattleSwitcher(const std::string& map, const std::string& chipset, PokemonGameEventDispatcher& ged, ska::Point<int> fightPos, const FightComponent& fc) :
SceneSwitcher(map, chipset),
m_fightComponent(fc),
m_fightPos(fightPos), 
m_ged(ged) {
}

void SceneToBattleSwitcher::switchTo(ska::Window& w, ska::SceneHolder& holder, ska::Scene&, ska::InputContextManager& icm, WorldScene& ws) const {
	auto scene = ska::ScenePtr(std::unique_ptr<ska::Scene>(std::make_unique<SceneFight>(w, holder, ws, icm, m_fightPos, m_fightComponent, m_ged)));
	holder.nextScene(scene);
}
