#include <fstream>
#include "Inventory.h"
#include "ChargementImages.h"
#include "WGameCore.h"
#include "IDs.h"


using namespace std;

Inventory::Inventory()
{

    m_font = TTF_OpenFont(FILE_FONT, 12);
    if(m_font == NULL)
        cerr << "Erreur lors de l'ouverture de la police : " << TTF_GetError() << endl;
    m_color.r = 128;
    m_color.g = 0;
    m_color.b = 0;
}

void Inventory::load(string squareSpriteName, string squareSpriteNameHighlight)
{
	m_squareSprite.load(squareSpriteName, T_RED, T_GREEN, T_BLUE);
	m_squareSpriteHighlight.load(squareSpriteNameHighlight, T_RED, T_GREEN, T_BLUE);
}

int Inventory::search(string name)
{
    list<unique_ptr<Object>>::iterator iter;
    int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end(); ++iter, i++)
	if(name == (*iter)->getName())
		return i;
    return -1;
}


void Inventory::swap(unsigned int index1, unsigned int index2)
{
    list<unique_ptr<Object>>::iterator iter1 = m_objects.begin(), iter2 = m_objects.begin();
    list<unsigned int>::iterator iterAmount1 = m_amount.begin(), iterAmount2 = m_amount.begin();
    list<Texture>::iterator iterAmountS1 = m_amountSurface.begin(), iterAmountS2 = m_amountSurface.begin();
    for(unsigned int i = 0; i < index1 && iter1 != m_objects.end(); iter1++, i++);
    for(unsigned int i = 0; i < index2 && iter2 != m_objects.end(); iter2++, i++);
    for(unsigned int i = 0; i < index1 && iterAmount1 != m_amount.end(); iterAmount1++, i++);
    for(unsigned int i = 0; i < index2 && iterAmount2 != m_amount.end(); iterAmount2++, i++);
    for(unsigned int i = 0; i < index1 && iterAmountS1 != m_amountSurface.end(); iterAmountS1++, i++);
    for(unsigned int i = 0; i < index2 && iterAmountS2 != m_amountSurface.end(); iterAmountS2++, i++);
    if(iter1 != iter2)
    {
        iter_swap(iter1, iter2);
        iter_swap(iterAmount1, iterAmount2);
        iter_swap(iterAmountS1, iterAmountS2);

    }

}

int Inventory::search(int id)
{
    list<unique_ptr<Object>>::iterator iter;
    int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end(); ++iter, i++)
        if(*iter != NULL && id == (*iter)->getID())
            return i;
    return -1;
}

unsigned int Inventory::getSize()
{
    list<unique_ptr<Object>>::iterator iter;
    unsigned int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end(); ++iter, i++);
    return i;
}

void Inventory::clear()
{
    m_objects.clear();
    m_amount.clear();
    list<Texture>::iterator iterAmountS;
    for(iterAmountS = m_amountSurface.begin(); iterAmountS != m_amountSurface.end(); ++iterAmountS)
        (*iterAmountS).free();
    m_amountSurface.clear();
}

//Obtenir l'objet num�ro "index" dans l'inventaire
Object *Inventory::getObjectFromIndex(int index)
{
    list<unique_ptr<Object>>::iterator iter;
    int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end() && i != index; i++ ,++iter);
	
	if(i == index && !m_objects.empty())
		return (Object*) &(**iter);
	return NULL;
}


list<unique_ptr<Object>>::iterator Inventory::getIteratorFromIndex(int index)
{
    list<unique_ptr<Object>>::iterator iter;
    int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end() && i != index; i++ ,++iter);
    return iter;
}

int Inventory::getIndexFromObject(std::list<std::unique_ptr<Object>>::iterator iterIndex)
{
    list<unique_ptr<Object>>::iterator iter;
    unsigned int i = 0;
    for(iter = m_objects.begin(); iter != m_objects.end() && iter != iterIndex; i++ ,++iter);
    if(i != m_objects.size())
        return i;
    else
        return -1;
}

//Obtenir la quantit� num�ro "index" dans l'inventaire
list<unsigned int>::iterator Inventory::getIteratorAmountFromIndex(int index)
{
    list<unsigned int>::iterator iter;
    int i = 0;
    for(iter = m_amount.begin(); iter != m_amount.end() && i != index; i++ ,++iter);
    return iter;
}

unsigned int Inventory::getAmountFromIndex(int index)
{
    list<unsigned int>::iterator iter;
    int i = 0;
    for(iter = m_amount.begin(); iter != m_amount.end() && i != index; i++ ,++iter);
    return *iter;
}

Texture* Inventory::getAmountSurfaceFromIndex(int index)
{
    list<Texture>::iterator iter;
    int i = 0;
    for(iter = m_amountSurface.begin(); iter != m_amountSurface.end() && i != index; i++ ,++iter);
    return (Texture*) &(*iter);
}

list<Texture>::iterator Inventory::getIteratorAmountSurfaceFromIndex(int index)
{
    list<Texture>::iterator iter;
    int i = 0;
    for(iter = m_amountSurface.begin(); iter != m_amountSurface.end() && i != index; i++ ,++iter);
    return iter;
}

void Inventory::add(int id, unsigned int amount)
{
    int index = this->search(id);

    if(index == -1)
    {
        int invSize = this->getSize();
        list<unique_ptr<Object>>::iterator iter;
        for(iter = m_objects.begin(), index = 0; index < invSize && *iter != NULL; ++iter, index++);
        if(index < invSize)
            *iter = unique_ptr<Object>(new Object(id));
        else
        {
            m_objects.push_back(unique_ptr<Object>(new Object(id)));
            m_amount.push_back(amount);
			Texture buf;
			buf.loadFromText(m_font, intToStr(amount), m_color);
            m_amountSurface.push_back(buf);
        }


    }
    else
    {
        *(this->getIteratorAmountFromIndex(index)) += amount;
        (*(this->getIteratorAmountSurfaceFromIndex(index))).loadFromText(m_font, intToStr(*(this->getIteratorAmountFromIndex(index))), m_color);
    }

}


void Inventory::remove(int id, unsigned int amount)
{
    int index = this->search(id);
    if(index == -1)
        return;

    //Si on retire une quantit� d'objet plus �lev�e que celle que l'on poss�de
    if((*(this->getIteratorAmountFromIndex(index))) - amount <= 0)
    {
        //On efface l'objet
        m_objects.erase(this->getIteratorFromIndex(index));
        m_amount.erase(this->getIteratorAmountFromIndex(index));
        m_amountSurface.erase(this->getIteratorAmountSurfaceFromIndex(index));
    }
    else
    {
        *(this->getIteratorAmountFromIndex(index)) -= amount;
        (*(this->getIteratorAmountSurfaceFromIndex(index))).loadFromText(m_font, intToStr(*(this->getIteratorAmountFromIndex(index))), m_color);
    }

}

void Inventory::remove(std::string name, unsigned int amount)
{
    int index = this->search(name);
    if(index == -1)
        return;

    if((*(this->getIteratorAmountFromIndex(index))) - amount <= 0)
    {
        m_objects.erase(this->getIteratorFromIndex(index));
        m_amount.erase(this->getIteratorAmountFromIndex(index));
        m_amountSurface.erase(this->getIteratorAmountSurfaceFromIndex(index));
    }
    else
    {
        *(this->getIteratorAmountFromIndex(index)) -= amount;
        (*(this->getIteratorAmountSurfaceFromIndex(index))).loadFromText(m_font, intToStr(*(this->getIteratorAmountFromIndex(index))), m_color);
    }

}

void Inventory::display(SDL_Rect rect)
{
	WGameCore& wScreen = WGameCore::getInstance();
	MouseInput *in = wScreen.getInputListener().getMouseInput();
    list<unique_ptr<Object>>::iterator iter;
    SDL_Rect buf, bufcenter, mouseClickPos;
    int i;

    for(iter = m_objects.begin(), i = 0; iter != m_objects.end(); ++iter, i++)
    {
        if(*iter != NULL)
        {

            buf.x = rect.x + (i * m_squareSprite.getWidth())%rect.w;
            buf.y = rect.y + (i * m_squareSprite.getWidth())/rect.w;
            buf.w = m_squareSprite.getWidth();
            buf.h = m_squareSprite.getHeight();
            bufcenter.w = (*iter)->getSprite()->getWidth()/2;
            bufcenter.h = (*iter)->getSprite()->getHeight();
            bufcenter = PosToCenterPicture(&bufcenter, &buf);
            bufcenter.x -= bufcenter.w/4;
            bufcenter.y += bufcenter.h/4;
            (*iter)->setPos(bufcenter.x, bufcenter.y);
            mouseClickPos = in->getMouseClickPos();
            if(IsPositionInBox(&mouseClickPos, &buf))
                m_squareSpriteHighlight.render(buf.x, buf.y);
            else
                m_squareSprite.render(buf.x, buf.y);

            buf.x += m_squareSprite.getWidth()/2;
            buf.y += m_squareSprite.getHeight()/8;
			(*this->getIteratorAmountSurfaceFromIndex(i)).render(buf.x, buf.y);
            
            (*iter)->display();
        }
    }

}

Texture* Inventory::getSquareSprite()
{
    return &m_squareSprite;
}

void Inventory::use(int id, unsigned int amount)
{
    int index = this->search(id);
    if(index == -1)
        return;
    for(unsigned int i = 0; i < amount; i++)
        if(this->getObjectFromIndex(index)->use())
			this->remove(id, 1);
}

void Inventory::use(int id, unsigned int amount, int indexPkmn)
{
    int index = this->search(id);
    if(index == -1)
        return;

    if(this->getObjectFromIndex(index)->getEffect() == "onPkmn")
        for(unsigned int i = 0; i < amount; i++)
			if(this->getObjectFromIndex(index)->use(indexPkmn))
				this->remove(id, 1);
    else
        for(unsigned int i = 0; i < amount; i++)
            if(this->getObjectFromIndex(index)->use())
				this->remove(id, 1);
}

Inventory::~Inventory()
{
    //TTF_CloseFont(m_font);
}