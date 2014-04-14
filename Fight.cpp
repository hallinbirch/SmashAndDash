#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>

#include "Fight.h"
#include "GestionEvents.h"
#include "Scrolling.h"
#include "ChargementImages.h"
#include "Skill.h"
#include "IDs.h"
#include "MouvementsMob.h"
#include "Statistics.h"

Fight::Fight(): m_animGrass(1, 3, false)
{
	SDL_Rect buf;
	WGameCore& wScreen = WGameCore::getInstance();
	buf.x = 0;
	buf.y = 0;
	buf.w = 48;
	buf.h = 16;
	m_animGrass.setOffsetAndFrameSize(buf);

    m_grassSprite.load("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Fight"FILE_SEPARATOR"grass.png", T_RED, T_GREEN, T_BLUE);
    m_pkmn = NULL;
    m_trainer = wScreen.getEntityFactory().getTrainer();
    m_opponent = NULL;
    m_isFighting = false;
    m_dialogActive = false;
    m_t0 = 0;
    m_duration = 0;

	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	pos.w = 8*TAILLEBLOCFENETRE;
	pos.h = 2*TAILLEBLOCFENETRE;

	m_mobAggroRange = 2*TAILLEBLOC;
    m_dial = unique_ptr<DialogMenu>(new DialogMenu("Dialogue", "", "."FILE_SEPARATOR"Menu"FILE_SEPARATOR"menu.png", pos, 22, false));
}

void Fight::setAreasFromLayerEvent()
{
	WGameCore& wScreen = WGameCore::getInstance();
	World& w = wScreen.getWorld();

    this->deleteAllAreas();
    for(int i = 0; i < w.getLayerEvent()->getNbrLignes(); i++)
    {
		if (w.getLayerEvent()->getAction(i) == "combat")
        {
            int width, height;
			ExtractTo(0, w.getLayerEvent()->getParam(i), ':', &width);
			height = atoi(w.getLayerEvent()->getParam(i).substr(w.getLayerEvent()->getParam(i).find_last_of(':') + 1, w.getLayerEvent()->getParam(i).size()).c_str());

			this->addArea(w.getLayerEvent()->getBlocX(i)*TAILLEBLOC, w.getLayerEvent()->getBlocY(i)*TAILLEBLOC, width*TAILLEBLOC, height*TAILLEBLOC);
        }
    }
}


bool Fight::isInFightArea(Player* hero)
{
    if(hero == NULL)
    {
        cerr << "Erreur (classe Fight) : Personnage invalide" << endl;
        return false;
    }

    SDL_Rect absolutePos = hero->getHitboxCenterPos();

    for(unsigned int i = 0; i < m_areaList.size(); i++)
        if(IsPositionInBox(&absolutePos, &(m_areaList[i])))
            return true;

    return false;
}

void Fight::addArea(int x, int y, int w, int h)
{
    SDL_Rect buf;
    buf.x = x;
    buf.y = y;
    buf.w = w;
    buf.h = h;
    m_areaList.push_back(buf);
}

void Fight::addArea(SDL_Rect area)
{
    m_areaList.push_back(area);
}

const SDL_Rect* Fight::getArea(int i)
{
    return &(m_areaList[i]);
}

void Fight::deleteAllAreas()
{
    m_areaList.clear();
}

void Fight::start(Character* opponent)
{
	WGameCore& wScreen = WGameCore::getInstance();
	World& w = wScreen.getWorld();
	int time;
	m_isFighting = true;


    m_pkmn = wScreen.getPokemonManager().getFirstUsablePokemon();

	if (m_pkmn == NULL || opponent == NULL)
		return;

	//Enregistrement dans les entit�s courantes du monde de l'apparition du Pok�mon et de son opposant
	wScreen.getEntityFactory().setPokemon(&(*m_pkmn));
	wScreen.getEntityFactory().setOpponent(opponent);

    m_opponent = opponent;
	m_opponentID.x = opponent->getID();
	m_opponentID.y = opponent->getEntityNumber();


    //Cr�ation de la barre de vie apparaissant en combat
    SDL_Rect pos; //sa position  >!! relative !!<
    pos.x = pos.y = 0;




	SDL_Rect posAnim;
	posAnim = m_opponent->getPos();
	wScreen.getSpriteAnimationManager().play(SPRITEBANK_ANIMATION, 2, posAnim, 1, 200);

	time = SDL_GetTicks();
	while(SDL_GetTicks() - time < 2000) // 2 secondes, le temps d'afficher l'animation de lancement de combat
	{
		wScreen.graphicUpdate();

		wScreen.flip();
		SDL_Delay(30);
	}
	
	m_dial->modifyText("Un " + m_opponent->getDescriptor()->getName() + " sauvage veut se battre !");
	SDL_Rect posDial = wScreen.getHero()->getPos();
	posDial.x += wScreen.getORel().x + posDial.w;
	posDial.y -= m_dial->getPos()->h - wScreen.getORel().y;
	m_dial->setPos(posDial);
	showDialog(2000);

	time = SDL_GetTicks();
	while(SDL_GetTicks() - time < 2000) // 2 secondes, le temps d'afficher le message de d�but de combat
	{
		wScreen.graphicUpdate();
		m_dial->display();

		wScreen.flip();
		SDL_Delay(30);
	}

    //wScreen.getGUI().getInfoPNJWindow()->hide(false);
    wScreen.getGUI().getFacesetPkmn()->hide(false);
    wScreen.getGUI().getAttackPokemonWindow()->hide(false);
    wScreen.getGUI().resetAttackPokemonWindow(m_pkmn);
    wScreen.getGUI().resetAttackOpponentWindow(m_opponent);
	
	SDL_Rect randomPos, relativePos, boxScreen;
	randomPos.x = wScreen.getHero()->getPos().x;
	randomPos.y = wScreen.getHero()->getPos().y;
	randomPos.h = wScreen.getHero()->getHeight();
	randomPos.w = wScreen.getHero()->getWidth();
	boxScreen.x = 0;
	boxScreen.y = 0;
	boxScreen.w = wScreen.getWidth();
	boxScreen.h = wScreen.getHeight();

	do 
	{
		do 
		{
			randomPos.x += rand()%(2*m_mobAggroRange + 1) - m_mobAggroRange;
			randomPos.y += rand()%(2*m_mobAggroRange + 1) - m_mobAggroRange;
			randomPos.x /= TAILLEBLOC;
			randomPos.x *= TAILLEBLOC;
			randomPos.y /= TAILLEBLOC;
			randomPos.y *= TAILLEBLOC;
			
			relativePos.x = randomPos.x + wScreen.getORel().x;
			relativePos.y = randomPos.y + wScreen.getORel().y;
		} while (!IsPositionInBox(&relativePos, &boxScreen));

	}while(wScreen.getWorld().getCollision(randomPos.x/TAILLEBLOC, randomPos.y/TAILLEBLOC) || !wScreen.detectEntity(randomPos).empty()); 
	

	m_opponent->setEntityNumber(ID_CURRENT_OPPONENT);
	m_opponent->setVisible(true);
	m_pkmn->setVisible(false);
    m_pkmn->teleport(randomPos.x, randomPos.y);

    wScreen.getPokeball().hide(true);
    wScreen.getPokeball().launch(wScreen.getHero(), m_pkmn->getPos());

    time = SDL_GetTicks();
    while(SDL_GetTicks() - time < 2000) // 2 secondes, le temps de lancer la pokeball par terre
    {
        wScreen.graphicUpdate();

        wScreen.flip();
        SDL_Delay(30);
    }


	m_pkmn->setVisible(true);
	m_pkmn->getHPBar()->setMaxValue(m_pkmn->getStatistics()->getHpMax());
	m_pkmn->getHPBar()->setCurrentValue(m_pkmn->getHp());
    m_pkmn->getHPBar()->setVisible(true);

    wScreen.setHero(m_pkmn);


}

void Fight::end(bool win)
{
	WGameCore& wScreen = WGameCore::getInstance();
    if(m_opponent == NULL || m_trainer == NULL)
    {
        m_isFighting = false;
        cerr << "Erreur (classe Fight) : un ou plusieurs participant au combat n'existe plus" << endl;
        return;
    }

	wScreen.getHero()->refreshStats();
	m_opponent->refreshStats();

    //Lancer de Pok�ball pour la fin du fight :
	m_opponent->getHPBar()->setVisible(false);
	m_pkmn->getHPBar()->setVisible(false);
	m_opponent->setVisible(false);
    wScreen.getPokeball().launch(m_trainer, wScreen.getHero()->getCenterPos());
    wScreen.getHero()->reset();

    //on attend 2s le temps que la Pok�ball aterrisse
    unsigned int duree = 2000, t0 = 0;
    t0 = SDL_GetTicks();

    while(SDL_GetTicks() - t0 < duree)
    {
        wScreen.graphicUpdate();
        //MobMovements();
        wScreen.flip();
        SDL_Delay(20);
    }



    //remettre le dresseur de pok�mon en tant que h�ro principal
    wScreen.setHero(m_trainer);
	m_trainer->getPath()->setPathString("");

    wScreen.getGUI().getInfoPNJWindow()->hide(true);
    wScreen.getGUI().getFacesetPkmn()->hide(true);
    wScreen.getGUI().getAttackPokemonWindow()->hide(true);

	m_opponent = NULL;
	m_pkmn = NULL;
	m_opponentID.x = 0;
	m_opponentID.y = 0;

    m_isFighting = false;

	if(!win)
	{
		wScreen.getWorld().changeLevel("poke1.bmp", "."FILE_SEPARATOR"Chipsets"FILE_SEPARATOR"chipset.png");
		wScreen.getHero()->teleport(8*TAILLEBLOC,5*TAILLEBLOC);
		wScreen.getPokemonManager().heal();
	}

}

void Fight::showDialog(unsigned int duration)
{
    m_t0 = SDL_GetTicks();
    m_duration = duration;
    m_dialogActive = true;
	m_dial->hide(false);
}

void Fight::display()
{
	WGameCore& wScreen = WGameCore::getInstance();
    if(m_isFighting)
    {
        if(m_pkmn != NULL)
            m_pkmn->displaySkills();
		if(m_opponent != NULL)
			m_opponent->displaySkills();



        //Refresh la barre de vie si active
        if(m_pkmn != NULL && m_pkmn->getHPBar()->isVisible())
        {
            SDL_Rect pos = m_pkmn->getPos();
            pos.y -= m_pkmn->getHPBar()->getPos().h + abs(wScreen.getORel().y);
            pos.x -= (m_pkmn->getWidth()/2 - m_pkmn->getHPBar()->getPos().w/2) + abs(wScreen.getORel().x);
            m_pkmn->getHPBar()->setPos(pos);
            m_pkmn->getHPBar()->refresh();
        }

        if(m_opponent != NULL && m_opponent->getHPBar()->isVisible())
        {
            SDL_Rect pos = m_opponent->getPos();
            pos.y -= m_opponent->getHPBar()->getPos().h + abs(wScreen.getORel().y);
            pos.x -= (m_opponent->getWidth()/2 - m_opponent->getHPBar()->getPos().w/2) + abs(wScreen.getORel().x);
            m_opponent->getHPBar()->setPos(pos);
            m_opponent->getHPBar()->refresh();
        }

    }

	this->displayDialog();
}

void Fight::refresh()
{
	WGameCore& wScreen = WGameCore::getInstance();

	if (m_isFighting)
	{
		wScreen.getAI().act(*this);
		if (wScreen.getHero() != NULL)
			wScreen.getHero()->refreshSkills();
		if (isFighting() && m_opponent != NULL)
			m_opponent->refreshSkills();
		
	}
	else
	{
	
		if(m_trainer->getSpeed() > 0)
			m_fightCount = rand()%(COMBAT_COUNT) + 1; //chiffre al�atoire entre 1 et COMBAT_COUNT (en fait, cela servira � d�clencher al�atoirement les combats en hautes herbes)

		//2 : chiffre au hasard entre 1 et COMBAT_COUNT
		if(m_fightCount == 2 && isInFightArea(m_trainer)) //Si on se trouve en zone de combat potentiel
		{
			m_fightCount = 0;
			IniReader* reader = GetRandomMobSettings(&wScreen.getWorld());

			if(reader != NULL)
				start(wScreen.getEntityFactory().createOpponent(reader));
		
		}
		else if(m_fightCount == 3)
		{
			m_fightCount = 0;
			vector<SDL_Rect> ids;
			SDL_Rect posTrainer = m_trainer->getPos();
			posTrainer.x -= m_mobAggroRange;
			posTrainer.y -= m_mobAggroRange;
			posTrainer.w = 2*m_mobAggroRange;
			posTrainer.h = 2*m_mobAggroRange;

			ids = wScreen.detectEntity(posTrainer);
			const size_t size = ids.size();
			for(size_t i = 0; i < size; i++)
				if(ids[i].x > 0 && (ids[i].x != m_trainer->getID() || ids[i].y != m_trainer->getEntityNumber()))  //Sinon si on est � proximit� d'un Pok�mon
				{
					start(wScreen.getEntityFactory().getNPC(ids[i].x, ids[i].y));
					return;
				}
		}
	}
}

void Fight::displayDialog()
{
	WGameCore& wScreen = WGameCore::getInstance();

    if(SDL_GetTicks() - m_t0 >= m_duration)
        m_dialogActive = false;

    if(m_dialogActive)
    {
		
        SDL_Rect pos = wScreen.getFight().getTrainer()->getPos();
        pos.x += wScreen.getORel().x + pos.w;
        pos.y -= m_dial->getPos()->h - wScreen.getORel().y;

        m_dial->setPos(pos);
        m_dial->display();
    }

}


Character* Fight::getOpponent()
{
    if(m_opponent == NULL)
        cerr << "Erreur (class Character_ptr) : Opposant NULL" << endl;
    return m_opponent;
}

Player* Fight::getPokemon()
{
    return m_pkmn;
}

Player* Fight::getTrainer()
{
    return m_trainer;
}

void Fight::setFightCount(int x)
{
    m_fightCount = x;
}

DialogMenu* Fight::getDialog()
{
	if(m_dial == NULL)
		cerr << "Erreur (class Character_ptr) : Fen�tre de dialogue du dresseur en combat NULL" << endl;
    return &(*m_dial);
}

int Fight::getFightCount()
{
    return m_fightCount;
}

void Fight::setFight(bool x)
{
    m_isFighting = x;
}

bool Fight::isFighting()
{
    return m_isFighting;
}

Fight::~Fight()
{
}

