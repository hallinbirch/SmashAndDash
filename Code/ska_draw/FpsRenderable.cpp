#include "FpsRenderable.h"
#include "StringUtils.h"
#include "TimeUtils.h"

ska::FpsRenderable::FpsRenderable() :
	m_t0(0), m_toggleVisible(true) {

}

void ska::FpsRenderable::update(const double value) {
	if (TimeUtils::getTicks() - m_t0 > DELAY) {
		m_fps.loadFromText(34, StringUtils::intToStr(static_cast<int>(value)), Color(255, 0, 0, 0));
		m_t0 = TimeUtils::getTicks();
	}
}

bool ska::FpsRenderable::isVisible() const {
	return m_toggleVisible;
}

void ska::FpsRenderable::setVisible(bool v) {
	m_toggleVisible = v;
}

void ska::FpsRenderable::display() const {
	m_fps.render(0, 0);
}
