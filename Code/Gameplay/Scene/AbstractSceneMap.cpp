#include "../WGameCore.h"
#include "../Weather.h"
#include "../World/World.h"
#include "../../Graphic/Scrolling.h"
#include "../../Script/ScriptDispatcher.h"
#include "../World/LayerE.h"
#include "AbstractSceneMap.h"

AbstractSceneMap::AbstractSceneMap()
{
}

void AbstractSceneMap::graphicUpdate(DrawableContainer& drawables) {
	WGameCore& core = WGameCore::getInstance();
	World& world = core.getWorld();
	GUI& gui = core.getGUI();
	

	AbstractNoGUISceneMap::graphicUpdate(drawables);

	//Affiche la GUI
	drawables.addHead(gui);

	//Affiche le Pok�mon ou l'objet sur le curseur de la souris
	//mouseCursor.displaySelectedPokemon();
	//mouseCursor.displaySelectedObject();

}

void AbstractSceneMap::eventUpdate(bool stuck) {
	WGameCore& core = WGameCore::getInstance();
	GUI& gui = core.getGUI();
	InputListener& kdListener = core.getInputListener();
	
	AbstractNoGUISceneMap::eventUpdate(stuck);

	//G�re les �v�nements ext�rieurs de la GUI (input)
	//GUI A PASSER EN TANT QU'OBSERVATEUR
	if (!kdListener.refresh(stuck)) {
		gui.getDialog()->hide(true);
		gui.getImgDialog()->hide(true);
		gui.hide(false);
	}

	//GUI
	gui.dialogRefresh();
	gui.refresh();

}

AbstractSceneMap::~AbstractSceneMap()
{
}