#include "CommandRandom.h"
#include "../../Utils/ScriptUtils.h"
#include "../../Utils/StringUtils.h"

ska::CommandRandom::CommandRandom(EntityManager& entityManager) : AbstractFunctionCommand(entityManager)
{
}


ska::CommandRandom::~CommandRandom()
{
}

int ska::CommandRandom::argumentsNumber() {
	return 1;
}

std::string ska::CommandRandom::execute(ScriptComponent&, std::vector<std::string>& args) {
	std::string s, value, commandCall;
	int valueInt, resultInt;

	value = args[0];
	valueInt = StringUtils::strToInt(value);

	if (valueInt > 0)
	{
		resultInt = rand() % valueInt;
		return StringUtils::intToStr(resultInt);
	}
	return "";
}
