#include <iostream>
#include "Button.h"
#include "ClickEvent.h"
#include "../Components/MouseObservable.h"

const std::string& ska::Button::MENU_DEFAULT_THEME_PATH = "."FILE_SEPARATOR"Menu"FILE_SEPARATOR"default_theme"FILE_SEPARATOR;

ska::Button::Button(Widget& parent, Point<int> relativePos, const std::string& placeHolderStyleName, const Rectangle* clip, ClickEventHandler const& callback) :
Hoverable<ValueChangedEventListener<bool>, ClickEventListener>(parent),
m_placeHolder(placeHolderStyleName + ".png"),
m_placeHolderHover(placeHolderStyleName + "_hover.png"),
m_placeHolderPressed(placeHolderStyleName + "_pressed.png"),
m_drawStyle(!placeHolderStyleName.empty()) {
	move(getRelativePosition() + relativePos);
	
	m_textureSelector = &m_placeHolder;
	m_lastTextureSelector = nullptr;
	setWidth(m_placeHolder.getWidth());
	setHeight(m_placeHolder.getHeight());
	
	initHandlers();
	addHandler<ClickEventListener>(callback);

	if (clip != nullptr) {
		m_clip = *clip;
	} else {
		memset(&m_clip, 0, sizeof m_clip);
		m_clip.w = std::numeric_limits<int>().max();
		m_clip.h = std::numeric_limits<int>().max();
	}

}

ska::Button::Button() : 
m_drawStyle(false) {
	m_textureSelector = &m_placeHolder;
	m_lastTextureSelector = nullptr;
	memset(&m_clip, 0, sizeof m_clip);
	m_clip.w = std::numeric_limits<int>().max();
	m_clip.h = std::numeric_limits<int>().max();

	initHandlers();
}

ska::Button::Button(Widget& parent) : 
Hoverable<ValueChangedEventListener<bool>, ClickEventListener>(parent),
m_drawStyle(false) {
	m_textureSelector = &m_placeHolder;
	m_lastTextureSelector = nullptr;
	memset(&m_clip, 0, sizeof m_clip);
	m_clip.w = std::numeric_limits<int>().max();
	m_clip.h = std::numeric_limits<int>().max();

	initHandlers();
}

void ska::Button::switchTextureAndMemorize() {
	Texture* t = nullptr;
	switch (m_state) {
		case ButtonState::HOVER:
		case ButtonState::ENTER:
			t = &m_placeHolderHover;
			break;

		case ButtonState::NONE:
		case ButtonState::DISABLED:
			t = &m_placeHolder;
			break;
		
		case ButtonState::PRESSED:
			t = &m_placeHolderPressed;
			break;

		default:
			t = &m_placeHolder;
			break;

	}
	m_lastTextureSelector = m_textureSelector;
	m_textureSelector = t;
}

void ska::Button::resetTexture() {
	m_textureSelector = m_lastTextureSelector;
}


void ska::Button::initHandlers() {
	addHeadHandler<ClickEventListener>([this](Widget*, ClickEvent& e) {
		bool handled = false;
		switch (e.getState()) {
		case MOUSE_CLICK:
			m_state = ButtonState::PRESSED;
			switchTextureAndMemorize();
			handled = true;
			break;
		case MOUSE_RELEASE:
			m_state = ButtonState::HOVER;
			resetTexture();
			handled = true;
			break;
		default:
			break;
		}

		if (handled) {
			e.setTarget(this);
		} else {
			e.stopPropagation(STOP_CALLBACK);
		}
	});
}

void ska::Button::forceState(ButtonState::Enum e) {
	m_state = e;
	switchTextureAndMemorize();
}

void ska::Button::display() const {
	if (!m_drawStyle || m_textureSelector == nullptr) {
		return;
	}

	const auto& pos = getAbsolutePosition();
	if (m_clip.w == std::numeric_limits<int>().max()) {
		m_textureSelector->render(pos.x, pos.y);
	} else {
		m_textureSelector->render(pos.x, pos.y, &m_clip);
	}
}



ska::Button::~Button() {

}

