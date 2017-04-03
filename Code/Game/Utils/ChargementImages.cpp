#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ChargementImages.h"
#include "World/World.h"
#include "Utils/NumberUtils.h"
#include "../Gameplay/Mobs/MobSpawner.h"

int GetRandom(std::vector<int>& probs)
{
	int r = rand()%(100);
	size_t newSize;
	std::vector<int> newProbs;

	const size_t size = probs.size();
	newSize = size;
	for(unsigned int i = 0; i < size && 1 < newSize; i++)
		if(r > probs[i])
		{
			probs[i] = -1;
			newSize--;
		}

	newProbs.reserve(newSize);
	for(unsigned int i = 0; i < size; i++)
		if(probs[i] != -1)
			newProbs.push_back(i);


	return newProbs[rand()%newSize];
}

ska::IniReader* GetRandomMobSettings(MobSpawner& w) {
	ska::IniReader* reader = NULL;
	std::vector<int> probs;
	const size_t totMobs = w.getMobSettings().size();

	probs.resize(totMobs);
	for(size_t i = 0; i < totMobs; i++)
		probs[i] = w.getMobSettings()[i].get<int>("Spawn probability_percent");


	if(totMobs != 0)
		reader = &w.getMobSettings()[GetRandom(probs)];

	return reader;
}

void VariablesAcquisition(std::vector<int> &vect, const std::string& filename)
{
	ska::IniReader reader(filename);
	int var_number = reader.get<int>("Game var_number");
	std::stringstream ss;

	if(var_number > 0)
		for(int i = 1; i <= var_number; i++)
		{
			ss << "%";

			for(int j = 4 - ska::NumberUtils::getMax10Pow(i); j >= 0; j--)
				ss << "0";

			ss << i;

			vect.push_back(reader.get<int>("Variable " + ss.str()));
			ss.str("");
		}

}

void SwitchesAcquisition(std::vector<bool> &vect, const std::string& filename)
{
	ska::IniReader reader(filename);
	int switch_number = reader.get<int>("Game switch_number");
	std::stringstream ss;

	if(switch_number > 0)
		for(int i = 1; i <= switch_number; i++)
		{
			ss << "%";

			for (int j = 4 - ska::NumberUtils::getMax10Pow(i); j >= 0; j--)
				ss << "0";

			ss << i;

			vect.push_back(reader.get<int>("Switch " + ss.str()) != 0);
			ss.str("");
		}

}

void VariablesWriting(const std::vector<int> &vect, const std::string& filename)
{
	ska::IniReader reader(filename);
	reader.set("Game var_number", vect.size());
	std::stringstream ss;

	for(unsigned int i = 1; i <= vect.size(); i++)
	{
		ss << "%";

		for (int j = 4 - ska::NumberUtils::getMax10Pow(i); j >= 0; j--)
			ss << "0";

		ss << i;

		reader.set("Variable " + ss.str(), vect[i-1]);
		ss.str("");
	}
	reader.save(filename);
}

void SwitchesWriting(const std::vector<bool> &vect, const std::string& filename)
{
	ska::IniReader reader(filename);
	reader.set("Game switch_number", vect.size());
	std::stringstream ss;

	for(unsigned int i = 1; i <= vect.size(); i++)
	{
		ss << "%";

		for (int j = 4 - ska::NumberUtils::getMax10Pow(i); j >= 0; j--)
			ss << "0";

		ss << i;

		reader.set("Switch " + ss.str(), vect[i-1]);
		ss.str("");
	}
	reader.save(filename);
}