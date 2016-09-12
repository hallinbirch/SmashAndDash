#include <iostream>
#include "Widget.h"

ska::Widget::Widget() :
	m_parent(nullptr),
	MouseObserver(std::bind(&ska::Widget::mouseHover, this, std::placeholders::_1), std::bind(&ska::Widget::click, this, std::placeholders::_1)) {
	m_box.x = 0;
	m_box.y = 0;
	m_box.w = 0;
	m_box.h = 0;
}

ska::Widget::Widget(Widget& parent) :
	m_parent(&parent), 
	MouseObserver(std::bind(&ska::Widget::mouseHover, this, std::placeholders::_1), std::bind(&ska::Widget::click, this, std::placeholders::_1)){
	m_box.x = 0;
	m_box.y = 0;
	m_box.w = 0;
	m_box.h = 0;
}

ska::Widget::Widget(Widget& parent, Point<int>& position) : 
	m_parent(&parent),
	MouseObserver(std::bind(&ska::Widget::mouseHover, this, std::placeholders::_1), std::bind(&ska::Widget::click, this, std::placeholders::_1)) {
	m_box.x = position.x;
	m_box.y = position.y;
	m_box.w = 0;
	m_box.h = 0;
}

void ska::Widget::setWidth(unsigned int w) {
	m_box.w = w;
}

void ska::Widget::setHeight(unsigned int h) {
	m_box.h = h;
}

const ska::Point<int> ska::Widget::getAbsolutePosition() const {
	if (m_parent == nullptr) {
		return m_box;
	}

	return m_parent->getAbsolutePosition() + ska::Point<int>(m_box);
}

bool ska::Widget::click(ska::ClickEvent& e) {
	auto affect = e.isOn(*this);
	if (!affect) {
		return false;
	}

	for (auto& ceh : m_clickCallbacks) {
		if (ceh != nullptr && (ceh)(e)) {
			return true;
		}
	}
	
	return false;
}


bool ska::Widget::mouseHover(ska::HoverEvent& e) {
	auto affect = e.isOn(*this);
	if (!affect) {
		return false;
	}

	for(auto& heh : m_hoverCallbacks) {
		/* Si un callback renvoie true, on stoppe */
		if (heh != nullptr && (heh)(e)) {
			break;
		}
	}
	return true;
}

const std::string& ska::Widget::getName() const {
	return m_name;
}

bool ska::Widget::isAParent(const ska::Widget& w) const {
	if(this == &w) {
		return true;
	}
	
	return w.getParent() == nullptr ? false : isAParent(*w.getParent());
}

void ska::Widget::setName(const std::string& s) {
	m_name = s;
}

void ska::Widget::addClickHandler(const ClickEventHandler& h) {
	m_clickCallbacks.push_back(h);
}

void ska::Widget::addHoverHandler(const HoverEventHandler& h) {
	m_hoverCallbacks.push_back(h);
}

const ska::Point<int> ska::Widget::getRelativePosition() const {
	return m_box;
}

void ska::Widget::show(bool sh) {
	m_visible = sh;
}

void ska::Widget::move(const ska::Point<int>& pos) {
	m_box.x = pos.x;
	m_box.y = pos.y;
}

bool ska::Widget::isVisible() const {
	return m_visible;
}

const ska::Widget* ska::Widget::getParent() const {
	return m_parent;
}

const ska::Rectangle& ska::Widget::getBox() const {
	return m_box;
}

