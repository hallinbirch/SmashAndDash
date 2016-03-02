#pragma once
#include <string>
#include <unordered_map>
#include "../Script/Script.h"
#include "../Data/Savegame.h"

namespace ska {
	class Savegame;
	class ScriptUtils
	{
	public:
		ScriptUtils();
		static std::string getValueFromVarOrSwitchNumber(const ska::Savegame& saveGame, const std::string& scriptExtendedName, std::string varNumber, std::unordered_map<std::string, std::string>& varMap);
		static std::string replaceVariablesByNumerics(const ska::Savegame& saveGame, const std::string& scriptExtendedName, const std::string& line, std::unordered_map<std::string, std::string>& varMap);
		static std::string replaceVariablesByNumerics(const ska::Savegame& saveGame, const std::string& scriptExtendedName, const std::string& line, std::unordered_map<std::string, std::string>& varMap, char varStartSymbol, char varEndSymbol);
		static std::string getFirstExpressionFromLine(Savegame& saveGame, const std::string& line, IScript* script, size_t* outputCommandSize);
		static std::string getCommandCall(const std::string& s);
		static std::string getVariableKey(const std::string& v);
		static void setValueFromVarOrSwitchNumber(ska::Savegame& saveGame, const std::string& scriptExtendedName, std::string varNumber, std::string value, std::unordered_map<std::string, std::string>& varMap);

		static std::string interpretVarName(const ska::Savegame& saveGame, const std::string& scriptExtendedName, std::string& v, std::unordered_map<std::string, std::string>& varMap);
		static bool isScriptActivated(const ska::Savegame& saveGame, const std::string& scriptName);
		~ScriptUtils();
	};
}