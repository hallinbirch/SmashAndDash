#ifndef DEF_GUI
#define DEF_GUI

#include <vector>
#include <memory>
#include "../../ska/Utils/Observer.h"
#include "../../ska/Utils/Observable.h"
#include "../../ska/Graphic/Draw/DrawableFixedPriority.h"
#include "../../ska/Graphic/Point.h"

using namespace std;

class WindowSettings;
class WindowTeam;
class WindowBag;
class ToolBar;
class WindowShop;
class WGameCore;
class DialogMenu;
class Character;
class IDialogMenu;

typedef std::unique_ptr<WindowBag> WindowBagPtr;
typedef std::unique_ptr<DialogMenu> DialogMenuPtr;
typedef std::unique_ptr<IDialogMenu> IDialogMenuPtr;
typedef std::unique_ptr<WindowSettings> WindowSettingsPtr;
typedef std::unique_ptr<WindowTeam> WindowTeamPtr;
typedef std::unique_ptr<WindowBag> WindowBagPtr;
typedef std::unique_ptr<ToolBar> ToolBarPtr;
typedef std::unique_ptr<WindowShop> WindowShopPtr;
typedef std::unique_ptr<Character> CharacterPtr;

class GUI : public ska::Observer<const int>, public ska::DrawableFixedPriority
{
    public:
    GUI();

	virtual void update(ska::Observable<const int>* obs, const ska::EventArg& e, const int& i) override;

    void dialogRefresh();
	void addDialog(IDialogMenuPtr& d);
    void refresh();
	int isPositionOnButton(const ska::Point<float>& pos);
    bool isMouseOnAWindow();
    bool isVisible() const override;
    void initButtons();
    void hide(bool x);
    bool isMovingAWindow();

	void resetInfoPNJWindow(Character* pnj);
	void resetInfoPokemonWindow(Character* pokemon);
	void resetAttackPokemonWindow(Character* pokemon);
    void resetAttackOpponentWindow(Character* op);

    void resetMovableWindowPokemonTeam(std::string action);
    void resetMovableWindowPokemonBag();
    void resetMovableWindowSettings();

	DialogMenuPtr& getMovableWindow();
	DialogMenuPtr& getInfoPNJWindow();
	DialogMenuPtr& getInfoPokemonWindow();
	DialogMenuPtr& getAttackPokemonWindow();
	DialogMenuPtr& getAttackOpponentWindow();
	DialogMenuPtr& getFacesetPkmn();
	DialogMenuPtr& getClickMenu();
	DialogMenuPtr& getFacesetOpponent();
    WindowSettingsPtr& getWindowSettings();
    WindowTeamPtr& getWindowTeam();
    WindowBagPtr& getWindowBag();
	ToolBarPtr& getToolbar();
    WindowShopPtr& getWindowShop();
    int getRefreshPNJWindowCount();
    DialogMenuPtr& getButton(unsigned int id);
    size_t getButtonListSize();

    void setRefreshPNJWindowCount(int x);
    void setPNJ(CharacterPtr *pnj);
    void setClickMenu();
	void dialogDisplay();
	void display() override;
private:
    vector<DialogMenuPtr> m_buttonList;
	vector<IDialogMenuPtr> m_extraWindows;
	DialogMenuPtr m_movableWindow, m_pnjInfoWindow, m_pokeInfoWindow, m_facesetPkmn, m_facesetOpponent, m_attackPokemon, m_attackOpponent, m_clickMenu;
    vector<int> m_buttonScroll;
	ska::Point<int> m_lastMousePos, m_curObjectPos;
    int m_side, m_lastMouseState, m_refreshCount;
    bool m_hide, m_isMovingWindow;
    Character *m_pnj;
    std::unique_ptr<WindowSettings> m_wSettings;
    std::unique_ptr<WindowTeam> m_wTeam;
    std::unique_ptr<WindowBag> m_wBag;
    std::unique_ptr<ToolBar> m_toolBar;
    std::unique_ptr<WindowShop> m_wShop;

};

#endif
