#ifndef DEF_WORLD
#define DEF_WORLD

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <windows.h>
#include <fmodex/fmod.h>
#include "../../Inputs/Readers/IniReader.h"
#include "../../Graphic/Texture.h"
#include "../../Graphic/Animation.h"


class Layer;
class LayerE;
class Weather;
class WGameCore;
class PhysicObject;
class Block;

class World
{
	public:
		World();
		void load(std::string fileName, std::string chipsetName, int windowWidth, int windowHeight);
		void displayLayers();
		Weather* getFog();
		Weather* getWeather();
		std::vector<IniReader>& getMobSettings();
		std::string getGenericName();
		int getNbrBlocX();
		int getNbrBlocY();
		Layer* getLayerBot();
		Layer* getLayerMid();
		Layer* getLayerTop();
		LayerE* getLayerEvent();
		Texture* getChipset();
		std::string getChipsetName();
		std::string getName();
		void getData();
		bool getCollision(const int i, const int j);
		bool isBlockDodgeable(const int i, const int j);

		void setSpriteFrame(unsigned int x);
		void setBgmVolume(int volPercent);
		void playBgm(bool x);
		void setBgm(std::string bgm);
		bool isBgmPlaying();
		bool canMoveToPos(SDL_Rect pos, PhysicObject* entityToMove);

		Block* getHigherBlock(const unsigned int i, const unsigned int j);
		void changeLevel(std::string fileName, std::string chipsetName);
		int spawnMob(SDL_Rect pos, unsigned int rmin, unsigned int rmax, unsigned int nbrSpawns, IniReader* dataSpawn);
		//void spawnMob(SDL_Rect pos, unsigned int rmin, unsigned int rmax, float distanceSpawns, unsigned int idMob);
		void refreshEntities();

		~World();
	
	private:
		void getFogFromData(std::string stringDataFile);
		void getRainFromData(std::string stringDataFile);
		void getWeatherFromData(std::string stringDataFile);
		void getBgmFromData(std::string stringDataFile);
		void getMobSettingsFromData();
	

		Texture m_chipset;
		FMOD_SOUND *m_bgm;
		FMOD_RESULT m_bgmResult;
		FMOD_CHANNELGROUP *m_canal;
		std::unique_ptr<Layer> m_lBot, m_lMid, m_lTop;
		std::unique_ptr<LayerE> m_lEvent;
		std::string m_chipsetName, m_fileName, m_genericName, m_worldName, m_botLayerName, m_midLayerName, m_topLayerName, m_eventLayerName, m_bgmName;
		std::unique_ptr<Weather> m_temps, m_brouillard;
		std::vector<IniReader> m_mobSettings;
		Animation m_animBlocks;

};

#endif