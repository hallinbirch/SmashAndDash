#include "GUIBattle.h"
#include "Bar.h"
#include "../../Utils\IDs.h"
#include "../../Gameplay/Data/RawStatistics.h"
#include "../../ska/Graphic/Draw/DrawableContainer.h"
#include "../../Gameplay/Fight/SkillsHolderComponent.h"
#include "../../ska/Graphic/SpritePath.h"
#include "../../ska/Graphic/GUI/Window.h"

GUIBattle::GUIBattle(ska::Window& w, StatisticsChangeObservable& statObs, BattleStartObservable& battleStartObs) :
StatisticsChangeObserver(std::bind(&GUIBattle::onStatisticsChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
BattleStartObserver(std::bind(&GUIBattle::onBattleStart, this, std::placeholders::_1, std::placeholders::_2)),
m_statsObservable(statObs),
m_battleStartObservable(battleStartObs),
m_moves("", ska::Rectangle {0, w.getHeight() - 4 * TAILLEBLOCFENETRE, 9*TAILLEBLOCFENETRE, 2 * TAILLEBLOCFENETRE}, -1, false) {
	m_statsObservable.addObserver(*this);
	m_battleStartObservable.addObserver(*this);
}

void GUIBattle::onStatisticsChange(const ska::EntityId& target, RawStatistics<int>& targetStats, const ska::EntityId& src) {
	if (m_bars.find(target) != m_bars.end()) {
		m_bars[target]->setCurrentValue(targetStats.hp);
	}
}

void GUIBattle::addHPBar(ska::CameraSystem& camSys, unsigned int maxValue, unsigned int currentValue, ska::EntityManager& em, const ska::EntityId& entityId) {
	m_bars[entityId] = std::move(BarPtr(new Bar(camSys, "."FILE_SEPARATOR"Menu"FILE_SEPARATOR"hpbar.png", "."FILE_SEPARATOR"Menu"FILE_SEPARATOR"hpbarcontent.png", maxValue, em, entityId)));
	m_bars[entityId]->setCurrentValue(currentValue);
}

void GUIBattle::onBattleStart(const ska::EntityId& target, SkillsHolderComponent& sh) {
	m_moves.deleteAll();
	m_moves.hide(false);
	ska::Point<int> buf;
	buf.x = TAILLEBLOCFENETRE / 4;
	buf.y = 0;
	auto& v = sh.skills;

	if (!v.empty()) {
		for (unsigned int i = 0; i < v.size(); i++) {
			buf.x += TAILLEBLOCFENETRE;
			m_moves.addImageArea(ska::SpritePath::getInstance().getPath(SPRITEBANK_ICONS, v[i].id), false, buf, nullptr);
		}
	}

}

void GUIBattle::eventUpdate(bool movingDisallowed) {
	if (m_moves.isVisible()) {
		m_moves.refresh();

		//On blit les cooldowns par dessus
		/*for(unsigned int i = 0; i < v->size(); i++) {
		if(!(*v)[i]->cooldownOK()) {
		ska::Texture* cooldownText;
		cooldownText = (*v)[i]->getSpriteRemainingCD();
		cooldownPos.x = m_attackPokemon->getRect().x + TAILLEBLOC / 2 + (i + 1) * (*v)[i]->getIcon()->getWidth();
		cooldownText->render(cooldownPos.x, cooldownPos.y);
		}
		}*/

	}
}

void GUIBattle::graphicUpdate(ska::DrawableContainer& drawables) {
	for (auto& b : m_bars) {
		drawables.add(*b.second);
	}
	drawables.add(m_moves);
}

void GUIBattle::clear() {
	m_bars.clear();
}

GUIBattle::~GUIBattle() {
	m_statsObservable.removeObserver(*this);
	m_battleStartObservable.removeObserver(*this);
	clear();
}