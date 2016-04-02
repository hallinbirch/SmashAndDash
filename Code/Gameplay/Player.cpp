#include "Player.h"
#include "../Gameplay\WGameCore.h"
#include "../Utils\ChargementImages.h"
#include "Mobs\GetIDmob.h"
#include "../ska/World\LayerE.h"
#include "../ska/Utils\StringUtils.h"
#include "../Utils/IDs.h"
#include "../ska/Utils/RectangleUtils.h"
#include "../Graphic/GUI/DialogMenu.h"

int GetSkillCursorFromKey(const ska::KeyEvent* ke);

Player::Player(int id) : Character(id)
{
	m_path.setPathString("");
}

void Player::update(ska::Observable<const int>* obs, const ska::EventArg& event, const int& i)
{
	const ska::MouseEvent* me;
	const ska::KeyEvent* ke;
	WGameCore& wScreen = WGameCore::getInstance();
	unsigned int power = getSpeedLimit() * 12;

	if (event.type() == EVENT_MOUSE)
	{
		me = static_cast<const ska::MouseEvent*>(&event);
		if (me->getButton() == SDL_BUTTON_RIGHT)
		{
			if (me->clicked())
			{
				//if (!wScreen.getFight().isFighting())
				{
					///////////////////////////////
					//CREE LE PATH DU PATHFINDING//
					///////////////////////////////

					ska::Rectangle dest;
					dest.x = me->getX();
					dest.y = me->getY();

/*
					dest.x -= wScreen.getORel().x;
					dest.y -= wScreen.getORel().y;*/

					dest.x = (dest.x / TAILLEBLOC) * TAILLEBLOC;
					dest.y = (dest.y / TAILLEBLOC) * TAILLEBLOC;

					// affichage du curseur pendant 3 secondes
					wScreen.getMouseCursor().showCursorTime(3000); 
					wScreen.getMouseCursor().setCursorPos(dest);


					//recalibrage de la position du hero pile sur un noeud pour eviter de le coincer contre des obstacles
					adaptPositionToBlock();

					m_path.setPathString("");
					findPath(dest);
					setCountD(0);
					getAnimation()->setCount(1);
				}
				/*else
				{
					//////////////////////////////////////////
					//VERIFIE S'IL EST POSSIBLE D'AUTOATTACK//
					//////////////////////////////////////////

					//ska::Rectangle opponentPos = wScreen.getFight().getOpponent()->getPos();
					ska::Rectangle opponentPos = { 0 };
					ska::Point<int> mouseRightClickPos;
					mouseRightClickPos.x = me->getX();
					mouseRightClickPos.y = me->getY();
					

					opponentPos.x += wScreen.getORel().x;
					opponentPos.y += wScreen.getORel().y;
					wScreen.getFight().getPokemon()->setAutoattack(ska::RectangleUtils::isPositionInBox(mouseRightClickPos, opponentPos));
				}*/

			}
		}
		else if (me->getButton() == SDL_BUTTON_LEFT)
		{

			//////////////////////////////////////////
			//DEPLACE LE PERSONNAGE VERS LE POINTEUR//
			//////////////////////////////////////////

			m_path.setPathString("");
			ska::Rectangle buf;
			buf.x = me->getX();
			buf.y = me->getY();
/*
			buf.x -= wScreen.getORel().x;
			buf.y -= wScreen.getORel().y;*/

			applyForce(buf, (float)power);
		}
	}
	else
	{
		//m_path.setPathString("");

		ke = static_cast<const ska::KeyEvent*>(&event);
		int direction = -1;

		if (ke->getScancode() == SDL_SCANCODE_W && ke->getSecondScancode() == SDL_SCANCODE_A)
			direction = 6;
		else if (ke->getScancode() == SDL_SCANCODE_W && ke->getSecondScancode() == SDL_SCANCODE_D)
			direction = 5;
		else if (ke->getScancode() == SDL_SCANCODE_S && ke->getSecondScancode() == SDL_SCANCODE_D)
			direction = 4;
		else if (ke->getScancode() == SDL_SCANCODE_S && ke->getSecondScancode() == SDL_SCANCODE_A)
			direction = 7;
		else if (ke->getScancode() == SDL_SCANCODE_W)
			direction = 2;
		else if (ke->getScancode() == SDL_SCANCODE_A)
			direction = 3;
		else if (ke->getScancode() == SDL_SCANCODE_S)
			direction = 0;
		else if (ke->getScancode() == SDL_SCANCODE_D)
			direction = 1;

		if (direction != -1)
		{
			setDirection(direction);
			applyForce(direction, (float)power);
		}

		//////////////////////////////////////////
		//COURIR								//
		//////////////////////////////////////////

		if (ke->getScancode() == SDL_SCANCODE_LSHIFT)
		{
			if (ke->getState() == 1)
			{
				getAnimation()->setDelay(75);
				run(true);
			}
			else
			{
				getAnimation()->setDelay(125);
				run(false);
			}
		}
		
		//////////////////////////////////////////
		//SAUTER								//
		//////////////////////////////////////////

		if (ke->getScancode() == SDL_SCANCODE_SPACE)
		{
			if (isOnGround())
				jump(18);
		}

		//////////////////////////////////////////
		//INTERACTION AVEC LES EVENEMENTS		//
		//////////////////////////////////////////

		if (ke->getScancode() == SDL_SCANCODE_RETURN /*&& !wScreen.getFight().isFighting()*/)
		{
			ska::World& w = wScreen.getWorld();
			/*wScreen.getGUI().getDialog()->hide(true);
			wScreen.getGUI().getImgDialog()->hide(true);*/

			ska::Rectangle buf;
			buf = GetIDmob(); //Fonction qui renvoie l'ID du mob � la position du h�ro et entite[ID] respectivement dans buf.x et buf.y
			

			if (buf.y != -ENTITEMAX)
			{
				/*int id = wScreen.getEntityFactory().getNPC(buf.x, buf.y)->getID();
				if (id >= 0){
					wScreen.getGUI().getImgDialog()->setMessImg("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Facesets"FILE_SEPARATOR"" + ska::StringUtils::intToStr(id) + ".png");
				} else {
					wScreen.getGUI().getImgDialog()->setMessImg("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Facesets"FILE_SEPARATOR"pnj" + ska::StringUtils::intToStr(-id) + ".png");
				}*/
					

				int j = 0, i;

				for (i = 1; i < w.getLayerEvent()->getNbrLignes(); i++)
				{

					if (w.getLayerEvent()->getID(i) == buf.x)
						j++;

					if (j - 1 == buf.y)
						break;

				}

				if (w.getLayerEvent()->getAction(i) == "script") {
					/* TODO : passer par "World" pour atteindre le ScriptDispatcher */
					/*ScriptDispatcher::getInstance().addRunningScript(NULL, w.getLayerEvent()->getParam(i), vector<string>(), 1);*/
				} /*else if (id < 0) {
					wScreen.getGUI().getDialog()->modifyText(w.getLayerEvent()->getParam(i));
					wScreen.getGUI().getDialog()->hide(false);
					wScreen.getGUI().getDialog()->display();
					wScreen.getGUI().getImgDialog()->hide(false);
					wScreen.getGUI().getImgDialog()->display();
				}*/
			}

		}
		//////////////////////////////////////////
		//LANCEMENT DES SKILLS					//
		//////////////////////////////////////////

		int skillCursor = GetSkillCursorFromKey(ke);
		if (skillCursor != -1)
		{
			const ska::InputRange& mousePos = wScreen.getRanges()[ska::InputRangeType::MousePos];
			ska::Point<int> absoluteMousePos = mousePos;
/*
			absoluteMousePos.x -= wScreen.getORel().x;
			absoluteMousePos.y -= wScreen.getORel().y;*/
			//wScreen.getHero()->launchSkill(skillCursor, absoluteMousePos);
		}
	}
}

void Player::refresh()
{
	if (!(m_id == 0 /*&& WGameCore::getInstance().getFight().isFighting()*/) && !m_path.getPathString().empty())
	{
		string path;
		setCountD(getCountD() - 1);
		path = m_path.getPathString();
		if (getCountD() <= 0)
		{

			setDirection(GetDirectionFromChar(path[0]));
			adaptPositionToBlock();

			if (!path.empty())
			{
				m_path.setPathString(path.substr(1, path.size()));

				setCountD(((float)TAILLEBLOC) / (getSpeedLimit() / 3 + 1));
				path = m_path.getPathString();
			}

		}
		else if (!path.empty())
		{
			reset();
			applyForce(GetDirectionFromChar(path[0]), (float)getSpeedLimit() * 5 * 10);
		}
	}

	Character::refresh();
}

Player::~Player()
{
}

int GetSkillCursorFromKey(const ska::KeyEvent* ke)
{
	int ret = -1;
	if (ke != NULL)
	{
		switch (ke->getScancode())
		{
		
			case SDL_SCANCODE_1:
				ret = 0;
				break;

			case SDL_SCANCODE_2:
				ret = 1;
				break;

			case SDL_SCANCODE_3:
				ret = 2;
				break;

			case SDL_SCANCODE_4:
				ret = 3;
				break;

		}
	}

	return ret;
}

