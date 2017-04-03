// #include "ToolBar.h"
// #include "../../Gameplay\Inventory\Inventory.h"
// //#include "../../Graphic\GUI\Inventory_Area.h"
// #include "../../Utils/IDs.h"
// #include "../../ska/Inputs/InputContextManager.h"
// 
// ToolBar::ToolBar(const ska::InputContextManager& icm, std::string squareSpriteName, std::string squareSpriteNameHighlight, std::string fichierMenu, ska::Rectangle posFond) :
// MovableWindow(icm, fichierMenu, posFond, 22)
// {
// 	ska::Rectangle pos;
// 	pos.x = TAILLEBLOCFENETRE/2;
// 	pos.y = TAILLEBLOCFENETRE/2;
// 	pos.w = posFond.w;
// 	pos.h = posFond.h;
// 
// 	m_inv.load(squareSpriteName, squareSpriteNameHighlight);
//     //addInventory(m_inv, pos);
//     //m_invArea = this->getInventoryArea(0);
// }
// 
// 
// void ToolBar::refresh()
// {
// 	const ska::InputActionContainer& in = m_playerICM.getActions();
// 
//     MovableWindow::refresh();
//     /*if(m_invArea != NULL)
//         m_invArea->refresh();
// 	else
// 		return;
// 
// 	Inventory* inv = m_invArea->getInventory();
// 	Object* obj;
// 
// 	if (in[ska::InputAction::UseObject1])
//     {
// 		obj = inv->getObjectFromIndex(0);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	else if (in[ska::InputAction::UseObject2])
//     {
// 		obj = inv->getObjectFromIndex(1);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	else if (in[ska::InputAction::UseObject3])
//     {
// 		obj = inv->getObjectFromIndex(2);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	else if (in[ska::InputAction::UseObject4])
//     {
// 		obj = inv->getObjectFromIndex(3);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	else if (in[ska::InputAction::UseObject5])
//     {
// 		obj = inv->getObjectFromIndex(4);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	else if (in[ska::InputAction::UseObject6])
//     {
// 		obj = inv->getObjectFromIndex(5);
// 		if(obj != NULL)
// 		{
// 			if(obj->getEffect() == "onPkmn")
// 				inv->use(obj->getID(), 1, 0);
// 			else 
// 				inv->use(obj->getID(), 1);
// 		}
//     }
// 	*/
// }
// 
// ToolBar::~ToolBar()
// {
// }