#include <iostream>
#include <string>

#include "Window_Area.h"
#include "DialogMenu.h"

using namespace std;

Window_Area::Window_Area(DialogMenu *parent)
{
    m_active = false;
    m_parent = parent;
    m_relativePos.w = 1;
    m_relativePos.h = 1;
    m_relativePos.x = 0;
    m_relativePos.y = 0;
}

SDL_Rect* Window_Area::getRelativePos()
{
    return &m_relativePos;
}

DialogMenu* Window_Area::getParent()
{
    return m_parent;
}

SDL_Rect Window_Area::getAbsolutePos()
{
    SDL_Rect buf = m_relativePos;
    buf.x += (m_parent->getPos())->x;
    buf.y += (m_parent->getPos())->y;
    return buf;
}

bool Window_Area::isVisible()
{
    return m_active;
}

void Window_Area::hide(bool x)
{
    m_active = !x;
}

bool Window_Area::isA(int type)
{
    return (m_type == type);
}

void Window_Area::setRelativePos(SDL_Rect pos)
{
    m_relativePos.x = pos.x;
    m_relativePos.y = pos.y;
}

bool Window_Area::canBeClicked()
{
    return m_canBeClicked;
}

Window_Area::~Window_Area(){}