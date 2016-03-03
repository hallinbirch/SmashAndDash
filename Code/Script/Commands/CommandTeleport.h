#pragma once
#include "../../ska/Script/Command/AbstractFunctionCommand.h"
class CommandTeleport :
	public ska::AbstractFunctionCommand
{
public:
	CommandTeleport();
	virtual ~CommandTeleport();

	static void teleportHeroToMap(std::string param);

	virtual std::string execute(ska::IScript* script, std::vector<std::string>& args) override;
	virtual int argumentsNumber();
};

