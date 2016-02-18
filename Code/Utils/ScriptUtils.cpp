#include "ScriptUtils.h"
#include "../Gameplay\WGameCore.h"
#include "FormalCalculation\FormalCalculator.h"
#include "../Script\ScriptSymbolsConstants.h"
#include "StringUtils.h"
#include "../Script\GlobalScriptVariables.h"
#include "../Script\ScriptDispatcher.h"
#include "../Exceptions/NumberFormatException.h"

using namespace std;

ScriptUtils::ScriptUtils()
{
}

/* R�cup�re la valeur une variable LOCALE (dans varMap) */
string ScriptUtils::getValueFromVarOrSwitchNumber(const string& scriptExtendedName, string varNumber, std::unordered_map<std::string, std::string>& varMap)
{
	int num = -1;
	WGameCore& wScreen = WGameCore::getInstance();

	if (varNumber[0] == '{' && varNumber[varNumber.size() - 1] == '}')
	{
		if (wScreen.getSavegameManager().getGameSwitch(StringUtils::strToInt(varNumber.substr(1, varNumber.size() - 2)) - 1)) {
			return "1";
		} else {
			return "0";
		}
	}
	else if (varNumber[0] == '[' && varNumber[varNumber.size() - 1] == ']')
	{
		string v = varNumber.substr(1, varNumber.size() - 2);
		string key = getVariableKey(v);
		if (!key.empty())
		{
			if (varMap.find(key) != varMap.end()) {
				return varMap[key];
			} else {
				return "";
			}
				
		}
		else {
			unsigned int varNum;
			try {
				varNum = StringUtils::strToInt(varNumber.substr(1, varNumber.size() - 2));
			} catch (NumberFormatException nfe) {
				return interpretVarName(scriptExtendedName, varNumber.substr(1, varNumber.size() - 2), varMap);
			}

			return StringUtils::intToStr(wScreen.getSavegameManager().getGameVariable(varNum - 1));
		}
			
	} else if (varNumber[0] == '#' && varNumber[varNumber.size() - 1] == '#') {
		const std::string& key = varNumber + scriptExtendedName;
		if (varMap.find(key) != varMap.end()) {
			return varMap[key];
		}
		else {
			return "";
		}
	}
	else if (varNumber == "true") {
		return "1";
	} else if (varNumber == "false") {
		return "0";
	}
	
	return varNumber;
}

string ScriptUtils::replaceVariablesByNumerics(const std::string& scriptExtendedName, const string& line, unordered_map<string, string>& varMap, char varStartSymbol, char varEndSymbol)
{
	string it = line;
	size_t posLeft, posRight;

	while ((posLeft = it.find_first_of(varStartSymbol)) != string::npos)
	{
		if ((posRight = it.substr(posLeft+1).find_first_of(varEndSymbol)) != string::npos)
		{
			posRight += posLeft + 1;

			string var = it.substr(posLeft, posRight - posLeft + 1);
			string varValue = ScriptUtils::getValueFromVarOrSwitchNumber(scriptExtendedName, var, varMap);

			it = it.substr(0, posLeft) + varValue + it.substr(posRight + 1, it.size());
		}
		else
		{
			FormalCalculator::calculSyntaxError(line);
			return "";
		}
	}

	return it;

}

string ScriptUtils::replaceVariablesByNumerics(const std::string& scriptExtendedName, const string& line, unordered_map<string, string>& varMap)
{
	string& it = replaceVariablesByNumerics(scriptExtendedName, line, varMap, ScriptSymbolsConstants::VARIABLE_LEFT, ScriptSymbolsConstants::VARIABLE_RIGHT);
	it = replaceVariablesByNumerics(scriptExtendedName, it, varMap, ScriptSymbolsConstants::ARG, ScriptSymbolsConstants::ARG);
	return it;
}

/*
Retourne la premi�re expression sur "line" apr�s calculs et formattage
Ex : si line = " [|bidule|] %random 100% [|chance|] %truc 200% " , la fonction va calculer "random 100" et renvoyer le r�sultat de ce calcul.
Ex : si line = " [|bidule|] %random %truc 200%% [|chance|]" , la fonction va calculer "random %truc 200%", lui-m�me va rappeler cette fonction et renvoyer le r�sultat de ce calcul total.
*/
std::string ScriptUtils::getFirstExpressionFromLine(const std::string& line, IScript* script, std::ifstream& fscript, size_t* outputCommandSize)
{
	size_t indexFirstChar;
	for (indexFirstChar = 0; line[indexFirstChar] != ScriptSymbolsConstants::METHOD && line[indexFirstChar] != '\n' && indexFirstChar < line.size(); indexFirstChar++);

	if (line[indexFirstChar] == '\n' || line.size() <= indexFirstChar) {
		if (outputCommandSize != NULL) {
			*outputCommandSize = 0;
		}
		return line;
	}
		

	const string& formattedLine = line.substr(indexFirstChar, line.size());
	const string& commandCall = ScriptUtils::getCommandCall(formattedLine);
	string valeur;

	if (outputCommandSize != NULL) {
		if (!commandCall.empty()) {
			*outputCommandSize = commandCall.size() + indexFirstChar + 2; //2 = les deux symboles '%' de la commande
		}
		else {
			*outputCommandSize = 0;
		}
	}

	if (!commandCall.empty()) {
		std::string result = ScriptDispatcher::commandInterpreter(script, commandCall, fscript);
		if (!result.empty()) {
			valeur = result;
		} else {
			valeur = formattedLine.substr(0, formattedLine.find_first_of(' '));
		}
	}
	else {
		valeur = formattedLine.substr(0, formattedLine.find_first_of(' '));
	}

	return valeur;
}

string ScriptUtils::getCommandCall(const string& s)
{
	if (!s.empty() && s[0] == ScriptSymbolsConstants::METHOD)
	{
		size_t posEndSubCmd = s.find_last_of(ScriptSymbolsConstants::METHOD);
		if (posEndSubCmd != std::string::npos)
			return s.substr(1, posEndSubCmd - 1);
	}

	return "";
}

string ScriptUtils::getVariableKey(const string& v)
{
	size_t pipePos = v.find_first_of('|');
	if (pipePos == 0 && v.find_last_of('|') == v.size() - 1)
	{
		//variable temporaire => varMap
		string key = "[" + v.substr(1, v.size() - 2) + "]";
		return key;
	}

	return "";
}

void ScriptUtils::setValueFromVarOrSwitchNumber(const string& scriptExtendedName, string varNumber, string value, std::unordered_map<std::string, std::string>& varMap)
{
	WGameCore& wScreen = WGameCore::getInstance();

	if (value.empty())
		return;

	if (varNumber[0] == '{' && varNumber[varNumber.size() - 1] == '}')
	{
		if (value == "true")
			wScreen.getSavegameManager().setGameSwitch(atoi(varNumber.substr(1, varNumber.size() - 2).c_str()) - 1, true);
		else
			wScreen.getSavegameManager().setGameSwitch(atoi(varNumber.substr(1, varNumber.size() - 2).c_str()) - 1, false);
	}
	else if (varNumber[0] == '[' && varNumber[varNumber.size() - 1] == ']')
	{
		string v = varNumber.substr(1, varNumber.size() - 2);
		string key = getVariableKey(v);
		if (!key.empty())
		{
			varMap[key] = value;
			return;
		}

	}
	else if (varNumber[0] == '#' && varNumber[varNumber.size() - 1] == '#') {
		varMap[varNumber + scriptExtendedName] = value;
	}
	
	wScreen.getSavegameManager().setGameVariable(atoi(varNumber.substr(1, varNumber.size() - 2).c_str()) - 1, atoi(value.c_str()));

}

/* R�cup�re la valeur d'une variable GLOBALE en utilisant potentiellement des sous-variables locales en param�tres */
std::string ScriptUtils::interpretVarName(const std::string& scriptExtendedName, std::string& v, std::unordered_map<std::string, std::string>& varMap)
{
	/*
	_variable_ : variable "constante" (int�gr�e au jeu)
	_variable param1 param2_ : variable int�gr�e au jeu avec param�tres
	#variable# : num�ro d'argument de script en cours entre symboles di�se
	|variable| : variable utilisateur (cr��e en script et utilis�e en script, morte � la fin du script)
	*/

	WGameCore& wScreen = WGameCore::getInstance();
	stringstream ss;
	string cmds[3];

	if (v[0] == '_' && v[v.size()-1] == '_')
	{
		ss << v.substr(1, v.size()-2);

		unsigned int i;
		for(i = 0; ss; i++) {
			ss >> cmds[i];
		}

		int cmdInt[2];
		cmdInt[0] = StringUtils::strToInt(getValueFromVarOrSwitchNumber(scriptExtendedName, cmds[1], varMap));
		cmdInt[1] = StringUtils::strToInt(getValueFromVarOrSwitchNumber(scriptExtendedName, cmds[2], varMap));

		return GlobalScriptVariables::getInstance().returnValue(cmds[0], cmdInt[0], cmdInt[1]);
	} 

	return getValueFromVarOrSwitchNumber(scriptExtendedName, v, varMap);
}

bool ScriptUtils::isScriptActivated(const string& scriptName)
{
	string s;
	WGameCore& wScreen = WGameCore::getInstance();
	ifstream scriptList(("."FILE_SEPARATOR"Data"FILE_SEPARATOR"Saves"FILE_SEPARATOR + wScreen.getSavegameManager().getSaveName() + FILE_SEPARATOR"scripts.data").c_str(), ios::app);
	ifstream tmpScriptList(("."FILE_SEPARATOR"Data"FILE_SEPARATOR"Saves"FILE_SEPARATOR + wScreen.getSavegameManager().getSaveName() + FILE_SEPARATOR"tmpscripts.data").c_str(), ios::app);

	if (scriptList.fail())
		return true;

	do
	scriptList >> s;
	while (s != scriptName && !scriptList.eof());


	if (s == scriptName)
		return false;
	else
	{
		s = "";
		if (tmpScriptList.fail())
			return true;

		do
		tmpScriptList >> s;
		while (s != scriptName && !tmpScriptList.eof());

		return !(s == scriptName);
	}
}

ScriptUtils::~ScriptUtils()
{
}
