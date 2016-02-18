#pragma once
#include "AbstractFunctionCommand.h"
class CommandPlayCrumbling :
	public AbstractFunctionCommand
{
public:
	CommandPlayCrumbling();
	virtual ~CommandPlayCrumbling();

	virtual std::string execute(IScript* script, std::vector<std::string>& args, std::ofstream& scriptList) override;
	virtual int argumentsNumber();
};

